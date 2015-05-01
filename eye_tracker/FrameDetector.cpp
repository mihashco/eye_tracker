#include "FrameDetector.h"


FrameDetector::FrameDetector()
{
	
}
 
FrameDetector::~FrameDetector()
{
}

void FrameDetector::moduleInit()
{
	eyeCalssifier.load("E:\\Dev\\opencv2411\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");
	faceClassifier.load("E:\\Dev\\opencv2411\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");

	//this->houghEyeCenterDetector.showSettingsWin();
}

void FrameDetector::moduleDeinit()
{
	//
}

void FrameDetector::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{
	this->benchmark.begin();

	if (this->testMode)
	{
		Point p = this->meansOfGradientsDetector.detect(srcFrame);
		cout << "Point X : " << p.x  << "Point y :" << p.y << std::endl;
		this->benchmark.end();
		return;
	}
		
	faceClassifier.detectMultiScale(srcFrame, faces, 1.2, 7, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	dstFrame = srcFrame.clone();

	for (int i = 0; i < faces.size(); i++)
	{
		Mat faceRoi = srcFrame(faces[i]);
		rectangle(dstFrame, faces[i], Scalar(255, 255, 255), 1);
		eyeCalssifier.detectMultiScale(faceRoi, eyes, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (int j = 0; j < eyes.size(); j++)
		{
			Mat eyeRoi = faceRoi(eyes[j]);
			rectangle(dstFrame, Rect(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y, eyes[j].width, eyes[j].height), Scalar(255, 255, 255), 1);
			//vector<Point> possibleCenters = this->houghEyeCenterDetector.detect(eyeRoi);

			//for (int k = 0; k < possibleCenters.size(); k++)
			//{
			//	circle(dstFrame, Point(faces[i].x + eyes[j].x + possibleCenters[k].x, faces[i].y + eyes[j].y + possibleCenters[k].y), 5, Scalar(255, 255, 255));
			//}

			Point p = this->meansOfGradientsDetector.detect(eyeRoi);
			std::cout << "X: [" << p.x << "] Y : [" << p.y << "]" << std::endl;
			circle(dstFrame, Point(faces[i].x + eyes[j].x + p.x, faces[i].y + eyes[j].y + p.y), 5, Scalar(255, 255, 255));
		}
	}

	this->benchmark.end();
}

void FrameDetector::moduleSetTestModeState(bool mode)
{
	this->testMode = mode;
}

bool FrameDetector::isTestModeOn()
{  
	return this->testMode;
}