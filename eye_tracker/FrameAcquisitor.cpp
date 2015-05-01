#include "FrameAcquisitor.h"
#include <stdio.h>

FrameAcquisitor::FrameAcquisitor()
{
	//this is the default configuration
	this->source = ACQUISITION_VIDEO_CAP;
}

FrameAcquisitor::~FrameAcquisitor()
{
}

void FrameAcquisitor::moduleInit()
{
	std::cout << "Moduele init! " << std::endl;

	if (this->source == ACQUISITION_IMAGES)
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
	else if (this->source == ACQUISITION_TEST_VIDEO)
	{
		cap.open("C:\\Users\\Michal\\Desktop\\test_data.mp4");
		if (!cap.isOpened())
		{
			std::cout << "Failed to read avi file!" << std::endl;
			return;
		}
	}
	else if (this->source == ACQUISITION_VIDEO_CAP)
	{
		cap.open(0);
		if (!cap.isOpened())
		{
			return;
		}

		cap.set(CV_CAP_PROP_FRAME_WIDTH, 1200);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 800);
	}
}

void FrameAcquisitor::moduleDeinit()
{
	cap.release();
}

void FrameAcquisitor::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{
	static int current_img = 0;

	if (this->source == ACQUISITION_VIDEO_CAP)
	{
		cap >> srcFrame;
	}
	else if (this->source == ACQUISITION_IMAGES)
	{
		if (current_img == imgPaths.size()) 
			current_img = 0;
		srcFrame = imread(imgPaths[current_img++], 1);
	}
	else if (this->source == ACQUISITION_TEST_VIDEO)
	{
		std::cout << "Loading frame" << std::endl;
		cap >> srcFrame;
	}
}

void FrameAcquisitor::acquistionSourceSet(AcqusitionSource src)
{
	this->source = src;
}