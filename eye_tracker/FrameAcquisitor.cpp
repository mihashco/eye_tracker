#include "FrameAcquisitor.h"
#include <stdio.h>

FrameAcquisitor::FrameAcquisitor()
{
	//this is the default configuration
	this->source = ACQUISITION_VIDEO_CAP;

	cap.open(0);
	if (!cap.isOpened())
	{
		//TODO:stop the application.
		//throw exception?
	}

	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 800);
}


FrameAcquisitor::~FrameAcquisitor()
{
}

void FrameAcquisitor::moduleInit()
{
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
}

void FrameAcquisitor::acquistionSourceSet(AcqusitionSource src)
{
	this->source = src;
}

/*
TODO:
1. If frame acquisitor source is : "Images" then it should use opencv function to load first image as a frame
2. Other modules should use this frame for calculations!
*/