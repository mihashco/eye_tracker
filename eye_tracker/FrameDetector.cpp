#include "FrameDetector.h"

FrameDetector::FrameDetector()
{
	//
}


FrameDetector::~FrameDetector()
{
}


void FrameDetector::moduleInit()
{
	eyeCalssifier.load("E:\\Dev\\opencv2411\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");
	faceClassifier.load("E:\\Dev\\opencv2411\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");
}

void FrameDetector::moduleDeinit()
{
	//
}

void FrameDetector::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{
	faceClassifier.detectMultiScale(srcFrame, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(40, 40));
	dstFrame = srcFrame.clone();

	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(dstFrame, faces[i], Scalar(255, 255, 255), 1);
	}
}