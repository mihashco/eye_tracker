#include "FramePreprocessor.h"
#include "ApplicationState.h"

FramePreprocessor::FramePreprocessor()
{
	
}


FramePreprocessor::~FramePreprocessor()
{

}

void FramePreprocessor::moduleInit(ApplicationState &appState)
{
	//
}

void FramePreprocessor::moduleDeinit()
{
	//
}

bool FramePreprocessor::moduleProcess(ApplicationState &appState)
{
	if (appState.frameSrc.empty())
		return false;

	cvtColor(appState.frameSrc, appState.frameGray, COLOR_BGR2GRAY);
	appState.frameGrayBlurred = appState.frameGray.clone();

	for (int i = 0; i < 35; i++) {
		blur(appState.frameGrayBlurred, appState.frameGrayBlurred, Size(5, 5));
	}

	return true;
}
