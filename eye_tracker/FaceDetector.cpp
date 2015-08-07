#include <Windows.h>

#include "FaceDetector.h"

FaceDetector::FaceDetector()
{
	namedWindow("Debug1", 0);
	moveWindow("Debug1", 0, 300);
	resizeWindow("Debug1", 426, 240);
}
 
FaceDetector::~FaceDetector()
{
}

void FaceDetector::moduleInit(ApplicationState &appState)
{
	faceClassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	eyeLclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml");
	eyeRclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_righteye_2splits.xml");
	eye2Lclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml");
	eye2Rclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_righteye_2splits.xml");
	noseClassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_nose.xml");
	mouthClassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_mouth.xml");
}

void FaceDetector::moduleDeinit()
{
	//
}

bool FaceDetector::moduleProcess(ApplicationState &appState)
{
	Point center;
	Point relCenterLeft;
	Point relCenterRight;

	int leftPx, leftPy, rightPx, rightPy;

	int biggestIdx = 0;
	int biggestArea = 0;

	/*Detect faces and gets the biggest to the rest processing*/
	faceClassifier.detectMultiScale(appState.frameGray, faces, 1.2, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(400, 400));

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

	/*Split the face to regions*/
	Rect r = faces[biggestIdx];
	Rect eyeArea = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaRight = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaLeft = Rect(r.x + r.width / 16 + (r.width - 2 * r.width / 16) / 2, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect noseArea = Rect(r.x + r.width / 6, r.y + r.height / 4 + r.height / 6, r.width / 6 * 4, r.height / 3 + r.height / 8);
	Rect mouthArea = Rect(r.x + r.width/ 8, r.y + r.height / 3 * 2 - r.height / 8, r.width / 4 * 3, r.height / 2);


	/*rectangle(appState.frameSrc, eyeAreaLeft.tl(), eyeAreaLeft.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameSrc, eyeAreaRight.tl(), eyeAreaRight.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameSrc, noseArea.tl(), noseArea.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameSrc, mouthArea.tl(), mouthArea.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameSrc, r, Scalar(255, 0, 0), 2);*/

	appState.faceMat = appState.frameGray(r);
	appState.faceRect = r;

	/*Detect rest face features*/
	eyeLclassifier.detectMultiScale(appState.frameGray(eyeAreaLeft), eyesLeft, 1.05, 5, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(100, 80));
	eyeLclassifier.detectMultiScale(appState.frameGray(eyeAreaRight), eyesRight, 1.05, 5, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(100, 80));
	noseClassifier.detectMultiScale(appState.frameGray(noseArea), nose, 1.05, 5, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE);
	mouthClassifier.detectMultiScale(appState.frameGray(mouthArea), mouth, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(100, 70), Size(160, 110));

	for (int i = 0; i < eyesLeft.size(); i++)
	{
		Rect e = eyesLeft[i];

		e.x = eyeAreaLeft.x + e.x;
		e.y = eyeAreaLeft.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.4, e.width, e.height* 0.6);

		rectangle(appState.frameGray, eyeRectangle, Scalar(255, 255, 255), 2);
		appState.lEyeMat = appState.frameGray(eyeRectangle);
		appState.lEyeCenterPoint = minDetector.detect(appState.frameGray(eyeRectangle), relCenterLeft, Point(e.x, e.y), leftPx, leftPy);
		appState.lEyeRelativeCenterPoint = Point(appState.lEyeCenterPoint.x + eyeRectangle.x, appState.lEyeCenterPoint.y + eyeRectangle.y);
		appState.lEyeCenterPoint = kFilterLeye.getPoint(appState.lEyeCenterPoint);
	}

	for (int i = 0; i < eyesRight.size(); i++)
	{
		Rect e = eyesRight[i];

		e.x = eyeAreaRight.x + e.x;
		e.y = eyeAreaRight.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.4, e.width, e.height* 0.6);

		appState.rEyeMat = appState.frameGray(eyeRectangle);
		appState.rEyeCenterPoint = minDetector.detect(appState.frameGray(eyeRectangle), relCenterRight, Point(e.x, e.y), rightPx, rightPy);
		appState.rEyeRelativeCenterPoint = Point(appState.rEyeCenterPoint.x + eyeRectangle.x, appState.rEyeCenterPoint.y + eyeRectangle.y);
		appState.rEyeCenterPoint = kFilterReye.getPoint(appState.rEyeCenterPoint);
	}

	for (int i = 0; i < nose.size(); i++) {
		Rect n = nose[i];
		rectangle(appState.frameSrc, Rect(noseArea.x + n.x, noseArea.y + n.y, n.width, n.height), Scalar(0, 0, 255), 2);
		appState.noseCenterPoint = Point(n.x + noseArea.x + n.width / 2, n.y + noseArea.y + n.height / 2);
		appState.noseAproxCenterPoint = kFilterNose.getPoint(appState.noseCenterPoint);
	}

	for (int i = 0; i < mouth.size(); i++) {
		Rect m = mouth[i];
		rectangle(appState.frameSrc, Rect(mouthArea.x + m.x, mouthArea.y + m.y, m.width, m.height), Scalar(0, 0, 255), 2);

		appState.mouthRect = m;
		appState.lmouthCornerPoint = Point(m.x + mouthArea.x, m.y + mouthArea.y + m.height / 2);
		appState.rmouthCornerPoint = Point(m.x + mouthArea.x + m.width, m.y + mouthArea.y + m.height /2);
		appState.mouthCenterPoint = Point(m.x + mouthArea.x + m.width / 2, m.y + mouthArea.y + m.height / 2);
		appState.mouthAproxCenterPoint = KFilterMouth.getPoint(appState.mouthCenterPoint);
	}

	Point usedEye;

	if (appState.usedEye == EYE_LEFT) {
		usedEye = appState.lEyeCenterPoint;
	}
	else {
		usedEye = appState.rEyeCenterPoint;
	}

	if (appState.isCalibrated) {
		appState.eyeDeltaX = appState.eyeMeanCenterPoint.x - usedEye.x;
		appState.eyeDeltaY = appState.eyeMeanCenterPoint.y - usedEye.y;
	}

	/*std::cout << "=========================================================" << std::endl;
	std::cout << "eyeDeltaX" << appState.eyeDeltaX << std::endl;
	std::cout << "eyeDeltaY" << appState.eyeDeltaY << std::endl;
	std::cout << "=========================================================" << std::endl;*/

	circle(appState.frameSrc, appState.lEyeRelativeCenterPoint, 10, Scalar(255, 0, 0), 3);
	circle(appState.frameSrc, appState.rEyeRelativeCenterPoint, 10, Scalar(0, 0, 255), 3);

	imshow("Debug1", appState.frameSrc);

	return true;
}