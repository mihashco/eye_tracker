#include <Windows.h>

#include "FaceAndEyeDetector.h"

FaceAndEyeDetector::FaceAndEyeDetector()
{
	namedWindow("Debug1", 0);
	moveWindow("Debug1", 0, 300);
	resizeWindow("Debug1", 426, 240);
}
 
FaceAndEyeDetector::~FaceAndEyeDetector()
{
}

void FaceAndEyeDetector::moduleInit(ApplicationState &appState)
{
	faceClassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	eyeLclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml");
	eyeRclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml");
}

void FaceAndEyeDetector::moduleDeinit()
{
	//
}

bool FaceAndEyeDetector::moduleProcess(ApplicationState &appState)
{
	Point center;
	Point relCenterLeft;
	Point relCenterRight;

	int leftPx, leftPy, rightPx, rightPy;

	int biggestIdx = 0;
	int biggestArea = 0;

	faceClassifier.detectMultiScale(appState.frameGray, faces, 1.2, 2, CV_HAAR_SCALE_IMAGE, Size(200, 200), Size(500, 500));

 	for (int i = 0; i < faces.size(); i++) 
	{
		Rect face = faces[i];
		if (face.area() > biggestArea)
		{
			biggestArea = face.area();
			biggestIdx = i;
		}
	}

	if (faces.empty())
		return true;

	Rect r = faces[biggestIdx];
	Rect eyeArea = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaRight = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaLeft = Rect(r.x + r.width / 16 + (r.width - 2 * r.width / 16) / 2, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));

	rectangle(appState.frameGray, eyeAreaLeft.tl(), eyeAreaLeft.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameGray, eyeAreaRight.tl(), eyeAreaRight.br(), Scalar(255, 255, 255), 2);

	appState.face = appState.frameGray(r);
	appState.faceRect = r;

	eyeLclassifier.detectMultiScale(appState.frameGray(eyeAreaLeft), eyesLeft, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(30, 30), Size(100, 100));
	eyeLclassifier.detectMultiScale(appState.frameGray(eyeAreaRight), eyesRight, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(30, 30), Size(100, 100));
	
	for (int i = 0; i < eyesLeft.size(); i++)
	{
		Rect e = eyesLeft[i];

		e.x = eyeAreaLeft.x + e.x;
		e.y = eyeAreaLeft.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.4, e.width, e.height* 0.6);

		rectangle(appState.frameGray, eyeRectangle, Scalar(255, 255, 255), 2);
		appState.lEye = appState.frameGray(eyeRectangle);
		appState.lEyeCenter = minDetector.detect(appState.frameGray(eyeRectangle), relCenterLeft, Point(e.x, e.y), leftPx, leftPy);
		appState.lEyeRelativeCenter = Point(appState.lEyeCenter.x + eyeRectangle.x, appState.lEyeCenter.y + eyeRectangle.y);
	}

	for (int i = 0; i < eyesRight.size(); i++)
	{
		Rect e = eyesRight[i];

		e.x = eyeAreaRight.x + e.x;
		e.y = eyeAreaRight.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.4, e.width, e.height* 0.6);
		rectangle(appState.frameGray, eyeRectangle, Scalar(255, 255, 255), 2);

		appState.rEye = appState.frameGray(eyeRectangle);
		appState.rEyeCenter = minDetector.detect(appState.frameGray(eyeRectangle), relCenterRight, Point(e.x, e.y), rightPx, rightPy);
		appState.rEyeRelativeCenter = Point(appState.rEyeCenter.x + eyeRectangle.x, appState.rEyeCenter.y+eyeRectangle.y);
	}

	circle(appState.frameSrc, appState.lEyeRelativeCenter, 10, Scalar(0, 0, 255), 3);
	circle(appState.frameSrc, appState.rEyeRelativeCenter, 10, Scalar(0, 0, 255), 3);

	imshow("Debug1", appState.frameSrc);
	//imshow("Debug2", appState.frameGray);

	return true;
}