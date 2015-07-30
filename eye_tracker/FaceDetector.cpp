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

	Rect r = faces[biggestIdx];
	Rect eyeArea = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaRight = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaLeft = Rect(r.x + r.width / 16 + (r.width - 2 * r.width / 16) / 2, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect noseArea = Rect(r.x + r.width / 6, r.y + r.height / 4, r.width / 6 * 4, r.height / 3 + r.height / 8);
	Rect mouthArea = Rect(r.x + r.width/ 8, r.y + r.height / 3 * 2 - r.height / 8, r.width / 4 * 3, r.height / 2);

	//rectangle(appState.frameSrc, eyeAreaLeft.tl(), eyeAreaLeft.br(), Scalar(255, 255, 255), 2);
	//rectangle(appState.frameSrc, eyeAreaRight.tl(), eyeAreaRight.br(), Scalar(255, 255, 255), 2);
	//rectangle(appState.frameSrc, noseArea.tl(), noseArea.br(), Scalar(255, 255, 255), 2);
	//rectangle(appState.frameSrc, mouthArea.tl(), mouthArea.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameSrc, r, Scalar(255, 0, 0), 2);

	appState.face = appState.frameGray(r);
	appState.faceRect = r;

	eyeLclassifier.detectMultiScale(appState.frameGray(eyeAreaLeft), eyesLeft, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(30, 30), Size(100, 100));
	eyeLclassifier.detectMultiScale(appState.frameGray(eyeAreaRight), eyesRight, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(30, 30), Size(100, 100));
	noseClassifier.detectMultiScale(appState.frameGray(noseArea), nose, 1.05, 5, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE);
	mouthClassifier.detectMultiScale(appState.frameGray(mouthArea), mouth, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(100, 70), Size(160, 110));

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

		appState.leftEyeLcorner = Point(e.x , e.y + e.height / 2);
		appState.leftEyeRcorner = Point(e.x + e.width / 2, e.y + e.height / 2);
	}

	for (int i = 0; i < eyesRight.size(); i++)
	{
		Rect e = eyesRight[i];

		e.x = eyeAreaRight.x + e.x;
		e.y = eyeAreaRight.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.4, e.width, e.height* 0.6);

		appState.rEye = appState.frameGray(eyeRectangle);
		appState.rEyeCenter = minDetector.detect(appState.frameGray(eyeRectangle), relCenterRight, Point(e.x, e.y), rightPx, rightPy);
		appState.rEyeRelativeCenter = Point(appState.rEyeCenter.x + eyeRectangle.x, appState.rEyeCenter.y+eyeRectangle.y);

		appState.rightEyeLcorner = Point(e.x, e.y + e.height / 2);
		appState.rightEyeRcorner = Point(e.x + e.width / 2, e.y + e.height / 2);
	}

	for (int i = 0; i < nose.size(); i++) {
		Rect n = nose[i];
		std::cout << "w : " << n.width << "h : " << n.height << std::endl;
		rectangle(appState.frameSrc, Rect(noseArea.x + n.x, noseArea.y + n.y, n.width, n.height), Scalar(0, 0, 255), 2);
		appState.noseCenter = Point(n.x + noseArea.x + n.width / 2, n.y + noseArea.y + n.height / 2);
	}

	for (int i = 0; i < mouth.size(); i++) {
		Rect m = mouth[i];
		rectangle(appState.frameSrc, Rect(mouthArea.x + m.x, mouthArea.y + m.y, m.width, m.height), Scalar(0, 0, 255), 2);

		std::cout << "mouthSize : X " << m.width << " Y : " << m.height << std::endl;
			 
		appState.mouthRect = m;
		appState.lmouthCorner = Point(m.x + mouthArea.x, m.y + mouthArea.y + m.height / 2);
		appState.rmouthCorner = Point(m.x + mouthArea.x + m.width, m.y + mouthArea.y + m.height /2);
		appState.mouthCenter = Point(m.x + mouthArea.x + m.width / 2, m.y + mouthArea.y + m.height / 2);
	}

	appState.headCenter = Point(appState.faceRect.x + appState.faceRect.width / 2, appState.faceRect.y + appState.faceRect.height / 2);



	circle(appState.frameSrc, appState.lEyeRelativeCenter, 10, Scalar(0, 0, 255), 3);
	circle(appState.frameSrc, appState.rEyeRelativeCenter, 10, Scalar(0, 0, 255), 3);


	imshow("Debug1", appState.frameSrc);

	return true;
}