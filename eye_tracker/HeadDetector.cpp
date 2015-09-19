#include "HeadDetector.h"


HeadDetector::HeadDetector()
{
	/*namedWindow("headPose", 0);
	resizeWindow("headPose", 320, 240);
	moveWindow("headPose", 0, 0);*/

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
	/*Calculate Head x, y position*/
	appState.headCenterPoint = Point(appState.faceRect.x + appState.faceRect.width / 2, appState.faceRect.y + appState.faceRect.height / 2);
	appState.headAproxCenterPoint = kFilter.getPoint(appState.headCenterPoint);

	/*Calculate HeadOX*/
	if (appState.headAngleRef == HEAD_MOUTH) {
		appState.headOY = appState.headAproxCenterPoint.x - appState.mouthAproxCenterPoint.x;
		appState.headOX = appState.headAproxCenterPoint.y - appState.mouthAproxCenterPoint.y;
	}
	else if (appState.headAngleRef == HEAD_NOSE) {
		appState.headOX = appState.headAproxCenterPoint.x - appState.noseAproxCenterPoint.x;
		appState.headOY = appState.headAproxCenterPoint.y - appState.noseAproxCenterPoint.y;
	}

	if (appState.isCalibrated) {
		appState.deltaHeadOX = appState.initHeadOX - appState.headOX;
		appState.deltaHeadOY = appState.initHeadOY - appState.headOY;
	}

	return true;
}

void HeadDetector::moduleDeinit(void)
{

}