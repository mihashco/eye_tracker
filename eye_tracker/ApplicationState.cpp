#include "ApplicationState.h"

ApplicationState::ApplicationState()
{
	this->acqSrc = ACQUISITION_VIDEO_CAP;
	this->camID = 0;
	this->frameWidth = 1280;
	this->frameHeight = 720;
}

ApplicationState::~ApplicationState()
{

}