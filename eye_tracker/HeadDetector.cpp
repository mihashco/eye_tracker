#include "HeadDetector.h"


HeadDetector::HeadDetector()
{
	
}


HeadDetector::~HeadDetector()
{
}

void HeadDetector::moduleInit(ApplicationState &appState)
{
}

bool HeadDetector::moduleProcess(ApplicationState &appState)
{
	appState.headCenter = Point(appState.faceRect.x + appState.faceRect.width / 2, appState.faceRect.y + appState.faceRect.height / 2);
	Mat axis = Mat::zeros(Size(appState.frameWidth, appState.frameHeight), appState.frameSrc.type());


	imshow("Axis", axis);
	return true;
}

void HeadDetector::moduleDeinit(void)
{

}
