#include "HeadDetector.h"


HeadDetector::HeadDetector()
{
	namedWindow("headPose", 0);
	resizeWindow("headPose", 320, 240);
	moveWindow("headPose", 0, 0);

	this->headCenterKF = new KalmanFilter(4, 2, 0);
	this->noseCenterKF = new KalmanFilter(4, 2, 0);
	this->mouthCenterKF = new KalmanFilter(4, 2, 0);

	this->kalmanFilterInit(this->headCenterKF);
	this->kalmanFilterInit(this->noseCenterKF);
	this->kalmanFilterInit(this->mouthCenterKF);

	this->rotation = Mat(2, 3, CV_32FC1);
	Point center = Point(1920/2, 1080/2);
	double angle = 180.0;
	double scale = 1.0;

	this->rotation = getRotationMatrix2D(center, angle, scale);
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
	setIdentity(kf->processNoiseCov, Scalar::all(1e-4));
	setIdentity(kf->measurementNoiseCov, Scalar::all(10));
	setIdentity(kf->errorCovPost, Scalar::all(.1));
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
	/*OUTPUT MATRIX FOR VISUALIZE COMPUTATIONS*/
	Mat headParams = Mat::zeros(Size(appState.frameWidth, appState.frameHeight), appState.frameSrc.type());
	Mat visibleRange = Mat::zeros(Size(1920, 1080), appState.frameSrc.type());
	Mat visibleRangeOut = Mat::zeros(Size(1920, 1080), appState.frameSrc.type());

	this->kalmanFilterGetAproximatedPoint(this->headCenterKF, appState.headCenter, appState.headAproxCenter);
	this->kalmanFilterGetAproximatedPoint(this->noseCenterKF, appState.noseCenter, appState.noseAproxCenter);
	this->kalmanFilterGetAproximatedPoint(this->mouthCenterKF, appState.mouthCenter, appState.mouthAproxCenter);

	/*OY angle calculation*/
	appState.headOY = appState.headAproxCenter.x - appState.mouthAproxCenter.x;
	/*OX angle calculation*/
	appState.headOX = appState.headAproxCenter.y - appState.mouthAproxCenter.y;
	/*OZ angle calculation*/

	circle(headParams, appState.headCenter, 25, Scalar(255, 0, 0), 3);
	circle(headParams, appState.headAproxCenter, 25, Scalar(0, 255, 0), 3);
	circle(headParams, appState.mouthCenter, 25, Scalar(0, 0, 255), 3);
	circle(headParams, appState.mouthAproxCenter, 25, Scalar(0, 255, 255), 3);
	
	int x = appState.headAproxCenter.x * 1.5;
	int y = appState.headAproxCenter.y * 1.5;
	
	circle(visibleRange, Point(x, y), 400, Scalar(0, 0, 255), 4);
	
	int deltaX = appState.oxForScreenCenter - appState.headOX;
	int deltaY = appState.oyForScreenCenter - appState.headOY;
	
	std::cout << "DELTAX : " << deltaX << std::endl;
	std::cout << "DELTAY : " << deltaY << std::endl;
	
	circle(visibleRange, Point(x + deltaY * 14, y + deltaX * 5), 400, Scalar(0, 255, 0), 8);
	
	warpAffine(visibleRange, visibleRangeOut, this->rotation, visibleRangeOut.size());
	flip(visibleRangeOut, visibleRangeOut, 0);
	imshow("headPose", visibleRangeOut);
	return true;
}

void HeadDetector::moduleDeinit(void)
{

}
