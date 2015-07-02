#include <stdio.h>

#include "EyeTracker.h"
#include "Timer.h"

#include "opencv2\gpu\gpu.hpp"

EyeTracker::EyeTracker()
{
	modulesCount = 4;

	//TODO: Each module should have the access to the application instance class.

	modules[0] = new FrameAcquisitor(); /*Camera initialization .edc*/
	modules[1] = new FramePreprocessor(); /*Image preprocessing and filtering for improve detection metods*/
	modules[2] = new FaceAndEyeDetector();

	//modules[3] = new HeadPosEstimator();
	modules[3] = new CursorController();
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
	for (;;)
	{
		int j = 0;
		for (j = 0; j < modulesCount; j++)
		{
			if (!modules[j]->moduleProcess(this->appState)) {
				std::cout << "Module [ " << j << "] failed" << std::endl;
				break;
			}
		}

		if (waitKey(1) == 30)
		{
			for (j = 0; j < 2; j++)
			{
				modules[j]->moduleDeinit();
			}
		}
	}
}