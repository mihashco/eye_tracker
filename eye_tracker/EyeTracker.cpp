#include <stdio.h>

#include "EyeTracker.h"


EyeTracker::EyeTracker()
{
	namedWindow("srcFrame", 1);
	namedWindow("dstFrame", 1);

	modulesCount = 3;

	modules[0] = new FrameAcquisitor();
	modules[1] = new FramePreprocessor();
	modules[2] = new FrameDetector();
}


EyeTracker::~EyeTracker()
{

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

		//printf("DBG: [%d][%d]\n", srcFrame.rows, srcFrame.cols);
		//printf("DBG: [%d][%d]\n", dstFrame.rows, dstFrame.cols);

		if (!srcFrame.empty())
		{
			imshow("srcFrame", srcFrame);
		}
		if (!dstFrame.empty())
		{
			imshow("dstFrame", dstFrame);
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