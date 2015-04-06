#include "FrameAcquisitor.h"
#include <stdio.h>

FrameAcquisitor::FrameAcquisitor()
{
	cap.open(0);
}


FrameAcquisitor::~FrameAcquisitor()
{
}

void FrameAcquisitor::moduleInit()
{
	printf("FrameAcquisitor::moduleInit()");
}

void FrameAcquisitor::moduleDeinit()
{
	cap.release();
}

void FrameAcquisitor::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{
	cap >> srcFrame;
}