#include "FramePreprocessor.h"


FramePreprocessor::FramePreprocessor()
{
	
}


FramePreprocessor::~FramePreprocessor()
{

}

void FramePreprocessor::moduleInit()
{
	//
}

void FramePreprocessor::moduleDeinit()
{
	//
}

void FramePreprocessor::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{
	if (srcFrame.empty()) return;

	//Stabilize face detector.
	GaussianBlur(srcFrame, srcFrame, Size(5, 5), 3, 3);

	cvtColor(srcFrame, dstFrame, COLOR_BGR2GRAY);
	srcFrame = dstFrame.clone();
}
