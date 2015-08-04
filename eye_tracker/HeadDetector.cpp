#include "HeadDetector.h"


HeadDetector::HeadDetector()
{
	namedWindow("headPose", 0);
	resizeWindow("headPose", 320, 240);
	moveWindow("headPose", 0, 0);

	this->headCenterKF = new KalmanFilter(4, 2, 0);
	this->noseCenterKF = new KalmanFilter(4, 2, 0);
	this->mouthCenterKF = new KalmanFilter(4, 2, 0);
	this->eyeCenterKF = new KalmanFilter(4, 2, 0);
	this->gazeOutKF = new KalmanFilter(4, 2, 0);

	this->kalmanFilterInit(this->headCenterKF);
	this->kalmanFilterInit(this->noseCenterKF);
	this->kalmanFilterInit(this->mouthCenterKF);
	this->kalmanFilterInit(this->eyeCenterKF);
	this->kalmanFilterInit(this->gazeOutKF);

	this->rotation = Mat(2, 3, CV_8U);
	Point center = Point(1920/2, 1080/2);
	double angle = 180.0;
	double scale = 1.0;

	this->rotation = getRotationMatrix2D(center, angle, scale);
	measureTime = 0;

	visited = Mat::zeros(Size(1920, 1080), CV_8UC1);
}

void HeadDetector::kalmanFilterInit(KalmanFilter *kf)
{
	kf->transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	Mat_<float> measurement(2, 1);
	measurement.setTo(Scalar(0));

	kf->statePre.at<float>(0) = 0;
	kf->statePre.at<float>(1) = 0;
	kf->statePre.at<float>(2) = 0;
	kf->statePre.at<float>(3) = 0;

	setIdentity(kf->measurementMatrix);
	setIdentity(kf->processNoiseCov, Scalar::all(1e-2));
	setIdentity(kf->measurementNoiseCov, Scalar::all(20));
	setIdentity(kf->errorCovPost, Scalar::all(1e-2));

	visited = Mat::zeros(Size(1920, 1080), CV_32SC1);
	checkedImage = imread("C:\\Users\\mszcz_000\\Desktop\\fhd\\test1.jpg");

}

void HeadDetector::kalmanFilterGetAproximatedPoint(KalmanFilter *kf, Point refPoint, Point &outPoint)
{
	if (!kf) return;
	
	Mat prediction = kf->predict();
	Mat_<float> measurement(2, 1);
	measurement.setTo(Scalar(0));

	measurement(0) = refPoint.x;
	measurement(1) = refPoint.y;

	Mat est = kf->correct(measurement);
	outPoint.x = est.at<float>(0);
	outPoint.y = est.at<float>(1);
}

HeadDetector::~HeadDetector()
{
	if (this->headCenterKF) delete this->headCenterKF;
}

void HeadDetector::moduleInit(ApplicationState &appState)
{
}

bool HeadDetector::moduleProcess(ApplicationState &appState)
{
	Mat headParams = Mat::zeros(Size(appState.frameWidth, appState.frameHeight), appState.frameSrc.type());
	Mat visibleRange = Mat::zeros(Size(1920, 1080), appState.frameSrc.type());
	Mat visibleRangeOut = Mat::zeros(Size(1920, 1080), appState.frameSrc.type());

	static int im_viewed = 0;

	this->kalmanFilterGetAproximatedPoint(this->headCenterKF, appState.headCenterPoint, appState.headAproxCenterPoint);
	this->kalmanFilterGetAproximatedPoint(this->noseCenterKF, appState.noseCenterPoint, appState.noseAproxCenterPoint);
	this->kalmanFilterGetAproximatedPoint(this->mouthCenterKF, appState.mouthCenterPoint, appState.mouthAproxCenterPoint);
	this->kalmanFilterGetAproximatedPoint(this->eyeCenterKF, appState.rEyeCenterPoint, appState.eyeAproxCenterPoint);

	if (appState.headAngleRef == HEAD_MOUTH) {
		appState.headOY = appState.headAproxCenterPoint.x - appState.mouthAproxCenterPoint.x;
		appState.headOX = appState.headAproxCenterPoint.y - appState.mouthAproxCenterPoint.y;
	}
	else if (appState.headAngleRef == HEAD_NOSE) {
		appState.headOY = appState.headAproxCenterPoint.x - appState.noseAproxCenterPoint.x;
		appState.headOX = appState.headAproxCenterPoint.y - appState.noseAproxCenterPoint.y;
	}

	int x = appState.headAproxCenterPoint.x * 1.5;
	int y = appState.headAproxCenterPoint.y * 1.5;
	
	circle(visibleRange, Point(x, y), 400, Scalar(0, 0, 255), 4);
	
	int deltaX = appState.oxForScreenCenter - appState.headOX;
	int deltaY = appState.oyForScreenCenter - appState.headOY;
	int eyeDeltaX = appState.eyeMeanCenterPoint.x - appState.eyeAproxCenterPoint.x;
	int eyeDeltaY = appState.eyeMeanCenterPoint.y - appState.eyeAproxCenterPoint.y;

	double eyeLeftFactor = 42;
	double eyeRightFactor = 80;

	double headLeftFactor = 17;
	double headRightFactor = 24;

	double usedEyeFactor;
	double usedHeadFactor;

	if (eyeDeltaX <= 0) {
		usedEyeFactor = eyeLeftFactor;
	}
	else {
		usedEyeFactor = eyeRightFactor;
	}

	if (eyeDeltaY <= 0) {
		usedHeadFactor = headLeftFactor;
	}
	else{
		usedHeadFactor = headRightFactor;
	}

	Point out(x - eyeDeltaX * usedEyeFactor + deltaY * usedHeadFactor, y - eyeDeltaY * 65 - deltaX * 2);

	this->kalmanFilterGetAproximatedPoint(this->gazeOutKF, out, appState.aproxGazePoint);

	if (appState.isCalibrated == true) {
		if (appState.aproxGazePoint.x > 0 && appState.aproxGazePoint.x <= appState.heatMapMat.size().width &&
			appState.aproxGazePoint.y >= 0 && appState.aproxGazePoint.y <= appState.heatMapMat.size().height) {

			uchar v = visited.at<uchar>(appState.aproxGazePoint);
			if(v < 245 ) v += 5;
			circle(visited, appState.aproxGazePoint, 40, Scalar(v), 80);
		}
	}

	blur(visited, visited, Size(9, 9));

	circle(headParams, appState.headCenterPoint, 25, Scalar(255, 0, 0), 3);
	circle(headParams, appState.headAproxCenterPoint, 25, Scalar(0, 255, 0), 3);
	circle(headParams, appState.mouthCenterPoint, 25, Scalar(0, 0, 255), 3);
	circle(headParams, appState.mouthAproxCenterPoint, 25, Scalar(0, 255, 255), 3);
	circle(visibleRange, appState.aproxGazePoint, 400, Scalar(0, 255, 0), 8);

	blur(appState.heatMapMat, appState.heatMapMat, Size(25, 25));

	warpAffine(visibleRange, visibleRangeOut, this->rotation, visibleRangeOut.size());
	flip(visibleRangeOut, visibleRangeOut, 0);

	imshow("headPose", visibleRangeOut);

	if (appState.isCalibrated == false) {
		imshow("headPose", visibleRangeOut);
	}
	else if(!im_viewed){
		imshow("TEST", checkedImage);
		//destroyWindow("headPose");
		im_viewed = 1;
	}

	if (appState.isCalibrated) {
		measureTime++;
	}


	std::cout << "======================================================================================" << std::endl;
	std::cout << "OUT : " << out << std::endl;
	std::cout << "APROX OUT : " << appState.aproxGazePoint << std::endl;
	std::cout << "HEAD OX :" << deltaX << "HEAD OY : " << deltaY << std::endl;
	std::cout << "EYE OX : " << eyeDeltaX << "EYE OY" << eyeDeltaY << std::endl;
	std::cout << "======================================================================================" << std::endl;
	
	if (measureTime == appState.measureTime) {
		computeHeatMap(appState);

		medianBlur(visited, visited, 25);
		applyColorMap(visited, visited, COLORMAP_HOT);

		warpAffine(visited, visited, this->rotation, visited.size());
		flip(visited, visited, 0);

		visited = checkedImage + visited;
		namedWindow("TEST", 1);
		moveWindow("TEST", 0, 0);
		imshow("TEST", visited);
	}

	return true;
}

void HeadDetector::moduleDeinit(void)
{

}

void HeadDetector::computeHeatMap(ApplicationState &appState) {
	double maxValue = 0;
	minMaxLoc(visited, NULL, &maxValue, NULL, NULL);
	std::cout << "Max Vaue " << maxValue << std::endl;
} 