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

	cv::resize(srcFrame, srcFrame, Size(1280, 800));

	cvtColor(srcFrame, dstFrame, COLOR_BGR2GRAY);
	srcFrame = dstFrame.clone();
}
