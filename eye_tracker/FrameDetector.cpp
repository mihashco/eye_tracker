#include <Windows.h>

#include "FrameDetector.h"

FrameDetector::FrameDetector()
{
	
}
 
FrameDetector::~FrameDetector()
{
}

void FrameDetector::moduleInit()
{
	eyeCalssifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");
	faceClassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");

	//this->houghEyeCenterDetector.showSettingsWin();
}

void FrameDetector::moduleDeinit()
{
	//
}

void FrameDetector::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{

//	this->benchmark.begin();
	faceClassifier.detectMultiScale(srcFrame, faces, 1.8, 7, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	dstFrame = srcFrame.clone();

	for (int i = 0; i < faces.size(); i++)
	{
		//detecting eyes only in the first half of the faceROI
		Mat faceRoi = srcFrame(Rect(faces[i].x, faces[i].y, faces[i].width / 2, faces[i].height / 2));

		rectangle(dstFrame, faces[i], Scalar(255, 255, 255), 1);
		eyeCalssifier.detectMultiScale(faceRoi, eyes, 1.1, 5, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (int j = 0; j < eyes.size(); j++)
		{
			Mat tmp = faceRoi(eyes[j]);
			Rect crop(0, tmp.size().height * 0.2, tmp.size().width, tmp.size().height * 0.7);
			Mat eyeRoi = tmp(crop);
			Point p = lDetector.detect(eyeRoi);
		}
	}
}

void FrameDetector::moduleSetTestModeState(bool mode)
{
	this->testMode = mode;
}

bool FrameDetector::isTestModeOn()
{
	return this->testMode;
}