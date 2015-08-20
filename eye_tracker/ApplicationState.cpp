#include "ApplicationState.h"

ApplicationState::ApplicationState()
{
	this->acqSrc = ACQUISITION_VIDEO_CAP;
	this->camID = 0;
	this->frameWidth = 1280;
	this->frameHeight = 720;
	
	this->headAngleRef = HEAD_NOSE;
	this->usedEye = EYE_LEFT;

	this->heatMapMat = Mat::zeros(Size(1920, 1080), 16);
	this->isCalibrated = false;

	this->measureTime = 200;
	this->gzMode = GAZE_TEST;
}

ApplicationState::~ApplicationState() 
{

}