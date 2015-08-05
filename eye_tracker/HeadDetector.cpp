#include "HeadDetector.h"


HeadDetector::HeadDetector()
{
	namedWindow("headPose", 0);
	resizeWindow("headPose", 320, 240);
	moveWindow("headPose", 0, 0);

	//this->rotation = Mat(2, 3, CV_8U);
	Point center = Point(1920/2, 1080/2);
	double angle = 180.0;
	double scale = 1.0;

	//this->rotation = getRotationMatrix2D(center, angle, scale);
	//measureTime = 0;

	//visited = Mat::zeros(Size(1920, 1080), CV_8UC1);
	//checkedImage = imread("C:\\Users\\mszcz_000\\Desktop\\fhd\\test1.jpg");
}

HeadDetector::~HeadDetector()
{
}

void HeadDetector::moduleInit(ApplicationState &appState)
{
}

bool HeadDetector::moduleProcess(ApplicationState &appState)
{
	//TODO: move this somewheere else
	/*Mat headParams = Mat::zeros(Size(appState.frameWidth, appState.frameHeight), appState.frameSrc.type());
	Mat visibleRange = Mat::zeros(Size(1920, 1080), appState.frameSrc.type());
	Mat visibleRangeOut = Mat::zeros(Size(1920, 1080), appState.frameSrc.type());*/

	//static int im_viewed = 0;

	
	//This is ok
	/*int x = appState.headAproxCenterPoint.x * 1.5;
	int y = appState.headAproxCenterPoint.y * 1.5;*/
	
	//This is not
	/*circle(visibleRange, Point(x, y), 400, Scalar(0, 0, 255), 4);
	
	int deltaX = appState.oxForScreenCenter - appState.headOX;
	int deltaY = appState.oyForScreenCenter - appState.headOY;*/
	
	//This is not ok
	/*int eyeDeltaX = appState.eyeMeanCenterPoint.x - appState.eyeAproxCenterPoint.x;
	int eyeDeltaY = appState.eyeMeanCenterPoint.y - appState.eyeAproxCenterPoint.y;

	double eyeLeftFactor = 42;
	double eyeRightFactor = 80;*/

	//This is ok
	/*double headLeftFactor = 17;
	double headRightFactor = 24;

	double usedEyeFactor;
	double usedHeadFactor;*/

	/*if (eyeDeltaX <= 0) {
		usedEyeFactor = eyeLeftFactor;
	}
	else {
		usedEyeFactor = eyeRightFactor;
	}*/

	/*if (eyeDeltaY <= 0) {
		usedHeadFactor = headLeftFactor;
	}
	else{
		usedHeadFactor = headRightFactor;
	}*/

	//Point out(x - eyeDeltaX * usedEyeFactor + deltaY * usedHeadFactor, y - eyeDeltaY * 65 - deltaX * 2);

	//this->kalmanFilterGetAproximatedPoint(this->gazeOutKF, out, appState.aproxGazePoint);

	/*if (appState.isCalibrated == true) {
		if (appState.aproxGazePoint.x > 0 && appState.aproxGazePoint.x <= appState.heatMapMat.size().width &&
			appState.aproxGazePoint.y >= 0 && appState.aproxGazePoint.y <= appState.heatMapMat.size().height) {

			uchar v = visited.at<uchar>(appState.aproxGazePoint);
			if(v < 245 ) v += 5;
			circle(visited, appState.aproxGazePoint, 40, Scalar(v), 80);
		}
	}*/

	//blur(visited, visited, Size(9, 9));

	/*circle(headParams, appState.headCenterPoint, 25, Scalar(255, 0, 0), 3);
	circle(headParams, appState.headAproxCenterPoint, 25, Scalar(0, 255, 0), 3);
	circle(headParams, appState.mouthCenterPoint, 25, Scalar(0, 0, 255), 3);
	circle(headParams, appState.mouthAproxCenterPoint, 25, Scalar(0, 255, 255), 3);
	circle(visibleRange, appState.aproxGazePoint, 400, Scalar(0, 255, 0), 8);*/

	/*blur(appState.heatMapMat, appState.heatMapMat, Size(25, 25));

	warpAffine(visibleRange, visibleRangeOut, this->rotation, visibleRangeOut.size());
	flip(visibleRangeOut, visibleRangeOut, 0);*/

	//imshow("headPose", visibleRangeOut);

	//if (appState.isCalibrated == false) {
	//	imshow("headPose", visibleRangeOut);
	//}
	//else if(!im_viewed){
	//	imshow("TEST", checkedImage);
	//	//destroyWindow("headPose");
	//	im_viewed = 1;
	//}

	/*if (appState.isCalibrated) {
		measureTime++;
	}*/
	
	/*if (measureTime == appState.measureTime) {
		computeHeatMap(appState);

		medianBlur(visited, visited, 25);
		applyColorMap(visited, visited, COLORMAP_HOT);

		warpAffine(visited, visited, this->rotation, visited.size());
		flip(visited, visited, 0);

		visited = checkedImage + visited;
		namedWindow("TEST", 1);
		moveWindow("TEST", 0, 0);
		imshow("TEST", visited);
	}*/


	/*Calculate Head x, y position*/
	appState.headCenterPoint = Point(appState.faceRect.x + appState.faceRect.width / 2, appState.faceRect.y + appState.faceRect.height / 2);
	appState.headAproxCenterPoint = kFilter.getPoint(appState.headCenterPoint);

	/*Calculate HeadOX*/
	if (appState.headAngleRef == HEAD_MOUTH) {
		appState.headOY = appState.headAproxCenterPoint.x - appState.mouthAproxCenterPoint.x;
		appState.headOX = appState.headAproxCenterPoint.y - appState.mouthAproxCenterPoint.y;
	}
	else if (appState.headAngleRef == HEAD_NOSE) {
		appState.headOY = appState.headAproxCenterPoint.x - appState.noseAproxCenterPoint.x;
		appState.headOX = appState.headAproxCenterPoint.y - appState.noseAproxCenterPoint.y;
	}

	if (appState.isCalibrated) {
		appState.deltaHeadOX = appState.initHeadOX - appState.headOX;
		appState.deltaHeadOY = appState.initHeadOY - appState.headOY;
	}

	/*std::cout << "======================================================================================" << std::endl;
	std::cout << "deltaHeadOX" << appState.deltaHeadOX << std::endl;
	std::cout << "deltaHeadOX" << appState.deltaHeadOY << std::endl;
	std::cout << "======================================================================================" << std::endl;*/
	
	return true;
}

void HeadDetector::moduleDeinit(void)
{

}

void HeadDetector::computeHeatMap(ApplicationState &appState) {
	double maxValue = 0;
	//minMaxLoc(visited, NULL, &maxValue, NULL, NULL);
	std::cout << "Max Vaue " << maxValue << std::endl;
} 