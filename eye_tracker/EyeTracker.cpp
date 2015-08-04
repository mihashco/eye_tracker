#include <stdio.h>

#include "EyeTracker.h"
#include "Timer.h"

EyeTracker::EyeTracker(int argc, char **argv)
{
	modulesCount = 5;

	modules[0] = new FrameAcquisitor(); /*Camera initialization .eTc*/
	modules[1] = new FramePreprocessor(); /*Image preprocessing and filtering for improve detection metods*/
	modules[2] = new FaceDetector();
	modules[3] = new HeadDetector();
	modules[4] = new Calibrator();
}

EyeTracker::~EyeTracker()
{
	this->applicationModulesDeinit();
}

void EyeTracker::applicationModulesInit()
{
	for (int i = 0; i < modulesCount; i++)
	{
		modules[i]->moduleInit(this->appState);
	}
}

void EyeTracker::applicationModulesDeinit()
{
	for (int i = 0; i < modulesCount; i++)
	{
		modules[i]->moduleDeinit();
		delete modules[i];
	}
}

void EyeTracker::startApplicationLoop()
{
	int j = 0;

	for (;;) {

		for (j = 0; j < modulesCount; j++)
		{
			if (!modules[j]->moduleProcess(this->appState)) {
				std::cout << "Module [ " << j << "] failed" << std::endl;
				break;
			}
		}

		if (waitKey(5) == 30)
		{
			for (j = 0; j < 2; j++)
			{
				modules[j]->moduleDeinit();
			}
		}
	}
}