#include "FrameAcquisitor.h"
#include <stdio.h>

FrameAcquisitor::FrameAcquisitor()
{
}

FrameAcquisitor::~FrameAcquisitor()
{
}

void FrameAcquisitor::moduleInit(ApplicationState &appState)
{
	if (appState.acqSrc == ACQUISITION_IMAGES)
	{
		imgPaths.push_back("E:\\EyeData\\img\\eye1.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye2.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye3.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye4.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye5.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye6.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye7.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye8.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye9.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye10.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye11.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye12.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye13.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye14.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye15.jpg");
		imgPaths.push_back("E:\\EyeData\\img\\eye16.jpg");
	}
	else if (appState.acqSrc == ACQUISITION_TEST_VIDEO)
	{
		cap.open("C:\\Users\\Michal\\Desktop\\test_data.mp4");
		if (!cap.isOpened())
		{
			std::cout << "Failed to read avi file!" << std::endl;
			return;
		}
	}
	else if (appState.acqSrc == ACQUISITION_VIDEO_CAP)
	{
		cap.open(appState.camID);
		if (!cap.isOpened())
		{
			return;
		}

		cap.set(CV_CAP_PROP_FRAME_WIDTH, appState.frameWidth);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, appState.frameHeight);
	}
}

void FrameAcquisitor::moduleDeinit()
{
	cap.release();
}

bool FrameAcquisitor::moduleProcess(ApplicationState &appState)
{
	static int current_img = 0;

	if (appState.acqSrc == ACQUISITION_VIDEO_CAP)
	{
		cap >> appState.frameSrc;
	}
	else if (appState.acqSrc == ACQUISITION_IMAGES)
	{
		if (current_img == imgPaths.size()) 
			current_img = 0;
		appState.frameSrc = imread(imgPaths[current_img++], 1);
	}
	else if (appState.acqSrc == ACQUISITION_TEST_VIDEO)
	{
		std::cout << "Loading frame" << std::endl;
		cap >> appState.frameSrc;
	}

	return true;
}