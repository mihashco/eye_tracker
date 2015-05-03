#include <stdio.h>

#include "EyeTracker.h"
#include "Timer.h"

#include "opencv2\gpu\gpu.hpp"

EyeTracker::EyeTracker()
{
	modulesCount = 3;

	modules[0] = new FrameAcquisitor();
	modules[1] = new FramePreprocessor();
	modules[2] = new FrameDetector();

	FrameAcquisitor *frAcquisitor = (FrameAcquisitor*) modules[0];
	frAcquisitor->acquistionSourceSet(ACQUISITION_VIDEO_CAP);

	FrameDetector *frDetector = (FrameDetector*)modules[2];
	frDetector->moduleSetTestModeState(false);

	//set window proprties;
	namedWindow("dstFrame", 0);
	namedWindow("srcFrame", 0);

	moveWindow("srcFrame", 0, 0);
	resizeWindow("srcFrame", 400, 320);

	moveWindow("dstFrame", 0, 320);
	resizeWindow("dstFrame", 400, 320);

	//std::cout << "EyeTracker Constructor" << gpu::getCudaEnabledDeviceCount() << std::endl;
}

EyeTracker::~EyeTracker()
{
	this->applicationModulesDeinit();
}

void EyeTracker::applicationModulesInit()
{
	for (int i = 0; i < modulesCount; i++)
	{
		modules[i]->moduleInit();
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
			modules[j]->moduleProcess(srcFrame, dstFrame);
		}

		if (!srcFrame.empty())
		{
			imshow("srcFrame", srcFrame);
		}
		if (!dstFrame.empty())
		{
			imshow("dstFrame", dstFrame);
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