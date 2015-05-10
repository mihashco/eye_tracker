#include "FrameDetector.h"

FrameDetector::FrameDetector()
{
	
}
 
FrameDetector::~FrameDetector()
{
}

void FrameDetector::moduleInit()
{
	eyeCalssifier.load("/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml");
	faceClassifier.load("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");

	this->houghDetector.showSettingsWin();
}

void FrameDetector::moduleDeinit()
{
	//
}

void FrameDetector::moduleProcess(Mat &srcFrame, Mat &dstFrame)
{
	// std::cout << "Width :" << srcFrame.size().width << " height " << srcFrame.size().height << std::endl;
	faceClassifier.detectMultiScale(srcFrame, faces, 1.8, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	dstFrame = srcFrame.clone();

	for (int i = 0; i < faces.size(); i++)
	{
		//detecting eyes only in the first half of the faceROI
		Mat faceRoi = srcFrame(Rect(faces[i].x, faces[i].y, faces[i].width / 2, faces[i].height / 2));

		rectangle(dstFrame, faces[i], Scalar(255, 255, 255), 1);
		eyeCalssifier.detectMultiScale(faceRoi, eyes, 1.1, 5, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (int j = 0; j < eyes.size(); j++)
		{
			Mat eyeRoi = faceRoi(eyes[j]);
			rectangle(dstFrame, Rect(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y, eyes[j].width, 
				eyes[j].height), Scalar(255, 255, 255), 1);
			Point p = this->houghDetector.detect(eyeRoi);
			if(!cal.moduleProcess(p.x, p.y))
			{
				//
			}
			else
			{
				cal.moveCursor(p.x, p.y);
			}
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