	#include <Windows.h>

#include "FaceDetector.h"

FaceDetector::FaceDetector()
{
	templAcquired = false;
	
	faceMinSizeX = 400;
	faceMaxSizeX = 1000;
	faceMinSizeY = 400;
	faceMaxSizeY = 1000;

	faceScale = 110;
	faceFactor = 2;

	lEyeMinSizeX = 86;
	lEyeMaxSizeX = 150;
	lEyeMinSizeY = 51;
	lEyeMaxSizeY = 120;

	lEyeScale = 110;
	lEyeFactor = 2;

	rEyeMinSizeX = 86;
	rEyeMaxSizeX = 150;
	rEyeMinSizeY = 51;
	rEyeMaxSizeY = 120;

	rEyeScale = 110;
	rEyeFactor = 2;
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
	eye2Rclassifier.load("E:\\Dev\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml");
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

	int biggestIdx = 0;
	int biggestArea = 0;

	/*Detect faces and gets the biggest to the rest processing*/
	//benchmark.begin();
	faceClassifier.detectMultiScale(appState.frameGrayBlurred, faces, (double) (faceScale / 100.0), faceFactor, 
										CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(faceMinSizeX,faceMinSizeY));
	for (int i = 0; i < faces.size(); i++)
	{
		Rect face = faces[i];
		if (face.area() > biggestArea)
		{
			biggestArea = face.area();
			biggestIdx = i;
		}
	}

	//std::cout << "Time: [" << benchmark.getElapsedMiliSeconds() << "] Count: [" << faces.size() << "] " << std::endl;

	if (faces.empty())
	{
		appState.noFace = true;
		return true;
	}
	appState.noFace = false;

	/*Split the face to regions*/
	Rect r = faces[biggestIdx];
	Rect eyeArea = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaRight = Rect(r.x + r.width / 16, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect eyeAreaLeft = Rect(r.x + r.width / 16 + (r.width - 2 * r.width / 16) / 2, (int)(r.y + (r.height / 4.5)), (r.width - 2 * r.width / 16) / 2, (int)(r.height / 3.0));
	Rect noseArea = Rect(r.x + r.width / 6, r.y + r.height / 4 + r.height / 6, r.width / 6 * 4, r.height / 3 + r.height / 8);
	Rect mouthArea = Rect(r.x + r.width/ 8, r.y + r.height / 3 * 2 - r.height / 8, r.width / 4 * 3, r.height / 2);

	rectangle(appState.frameSrc, eyeAreaLeft.tl(), eyeAreaLeft.br(), Scalar(255, 0, 0), 2);
	rectangle(appState.frameSrc, eyeAreaRight.tl(), eyeAreaRight.br(), Scalar(255, 0, 0), 2);
	rectangle(appState.frameSrc, noseArea.tl(), noseArea.br(), Scalar(255, 255, 255), 2);
	rectangle(appState.frameSrc, mouthArea.tl(), mouthArea.br(), Scalar(0, 255, 0), 2);
	rectangle(appState.frameSrc, r, Scalar(0, 0, 255), 2);

	appState.faceMat = appState.frameGrayBlurred(r);
	appState.faceRect = r;

	/*Detect rest face features*/
	eyeLclassifier.detectMultiScale(appState.frameGrayBlurred(eyeAreaLeft), eyesLeft, (double) (lEyeScale / 100.0), lEyeFactor, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(lEyeMinSizeX, lEyeMinSizeY));
	eyeRclassifier.detectMultiScale(appState.frameGrayBlurred(eyeAreaRight), eyesRight, (double)(rEyeScale / 100.0), rEyeFactor, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(rEyeMinSizeX, rEyeMinSizeY));
	noseClassifier.detectMultiScale(appState.frameGrayBlurred(noseArea), nose, 1.05, 5, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE);
	mouthClassifier.detectMultiScale(appState.frameGrayBlurred(mouthArea), mouth, 1.1, 2, CASCADE_FIND_BIGGEST_OBJECT | CASCADE_SCALE_IMAGE, Size(100, 70), Size(160, 110));

	for (int i = 0; i < eyesLeft.size(); i++)
	{
		Rect e = eyesLeft[i];

		e.x = eyeAreaLeft.x + e.x;
		e.y = eyeAreaLeft.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.5, e.width, e.height* 0.6);
		appState.lEyeMat = appState.frameGray(eyeRectangle);
		//appState.lEyeCenterPoint = this->minDetector.detect(appState.frameGray(eyeRectangle));
		//appState.lEyeRelativeCenterPoint = Point(appState.lEyeCenterPoint.x + eyeRectangle.x, appState.lEyeCenterPoint.y + eyeRectangle.y);
	}

	for (int i = 0; i < eyesRight.size(); i++)
	{
		Rect e = eyesRight[i];

		e.x = eyeAreaRight.x + e.x;
		e.y = eyeAreaRight.y + e.y;

		Rect eyeRectangle = Rect(e.tl().x, e.tl().y + e.height * 0.5, e.width, e.height* 0.6);
		appState.rEyeMat = appState.frameGray(eyeRectangle);
		//appState.rEyeCenterPoint = this->minDetector.detect(appState.frameGray(eyeRectangle));
		//appState.rEyeRelativeCenterPoint = Point(appState.rEyeCenterPoint.x + eyeRectangle.x, appState.rEyeCenterPoint.y + eyeRectangle.y);
	}

	appState.lEyeCenterPoint = this->minDetector.detect(appState.lEyeMat);
	appState.rEyeCenterPoint = this->minDetector.detect(appState.rEyeMat);

	for (int i = 0; i < nose.size(); i++) {
		Rect n = nose[i];
		rectangle(appState.frameSrc, Rect(noseArea.x + n.x, noseArea.y + n.y, n.width, n.height), Scalar(0, 0, 255), 2);
		appState.noseCenterPoint = Point(n.x + noseArea.x + n.width / 2, n.y + noseArea.y + n.height / 2);
		appState.noseAproxCenterPoint = kFilterNose.getPoint(appState.noseCenterPoint);
	}

	for (int i = 0; i < mouth.size(); i++) {
		Rect m = mouth[i];
		//rectangle(appState.frameSrc, Rect(mouthArea.x + m.x, mouthArea.y + m.y, m.width, m.height), Scalar(0, 0, 255), 2);

		appState.mouthRect = m;
		appState.lmouthCornerPoint = Point(m.x + mouthArea.x, m.y + mouthArea.y + m.height / 2);
		appState.rmouthCornerPoint = Point(m.x + mouthArea.x + m.width, m.y + mouthArea.y + m.height /2);
		appState.mouthCenterPoint = Point(m.x + mouthArea.x + m.width / 2, m.y + mouthArea.y + m.height / 2);
		appState.mouthAproxCenterPoint = KFilterMouth.getPoint(appState.mouthCenterPoint);
	}

	if (appState.usedEye == EYE_LEFT) {
		appState.eyeCenter = kFilterLeye.getPoint(appState.lEyeCenterPoint);
	}
	else {
		appState.eyeCenter = kFilterReye.getPoint(appState.rEyeCenterPoint);
	}

	Point ue = appState.eyeCenter;

	if (appState.isCalibrated) {
		appState.eyeDeltaX = appState.eyeMeanCenterPoint.x - ue.x;
		appState.eyeDeltaY = appState.eyeMeanCenterPoint.y - ue.y;
	}

	imshow("DEBUG1", appState.frameSrc);

	return true;
}

void FaceDetector::showSettingsWin(void)
{
	std::string winName = "FaceDetectorSettingsWin";
	namedWindow(winName,1);

	int faceMinSize;
	int faceMaxSize;
	int faceScale;
	int faceFactor;

	int lEyeMinSize;
	int lEyeMaxSize;
	int lEyeScale;
	int lEyeFeactor;

	int rEyeMinSize;
	int rEyeMaxSize;
	int rEyeScale;
	int rEyeFactor;

	createTrackbar("FaceMinSize", winName, &this->faceMinSizeX, 500);
	createTrackbar("FaceMinSize", winName, &this->faceMinSizeY, 500);
	createTrackbar("FaceMaxSize", winName, &this->faceMaxSizeX, 600);
	createTrackbar("FaceMaxSize", winName, &this->faceMaxSizeY, 600);

	createTrackbar("FaceScale", winName, &this->faceScale, 200);
	createTrackbar("FaceFactor", winName, &this->faceFactor, 20);
	
	createTrackbar("lEyeMinSize", winName, &this->lEyeMinSizeX, 120);
	createTrackbar("lEyeMinSize", winName, &this->lEyeMinSizeY, 100);
	createTrackbar("lEyeMaxSize", winName, &this->lEyeMaxSizeX, 200);
	createTrackbar("lEyeMaxSize", winName, &this->lEyeMaxSizeY, 150);

	createTrackbar("lEyeScalre", winName, &this->lEyeScale, 200);
	createTrackbar("lEyeFactor", winName, &this->lEyeFactor, 20);

	createTrackbar("rEyeMinSize", winName, &this->rEyeMinSizeX, 120);
	createTrackbar("rEyeMinSize", winName, &this->rEyeMinSizeY, 100);
	createTrackbar("rEyeMaxSize", winName, &this->rEyeMaxSizeX, 200);
	createTrackbar("rEyeMaxSize", winName, &this->rEyeMaxSizeY, 150);

	createTrackbar("rEyeScale", winName, &this->rEyeScale, 200);
	createTrackbar("rEyeFactor", winName, &this->rEyeFactor, 20);
}