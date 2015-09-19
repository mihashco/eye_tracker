#include "opencv2\opencv.hpp"
#include "opencv2\contrib\detection_based_tracker.hpp"
#include "AppModule.h"

#include "HoughDetector.h"
#include "MeansOfGradientsDetector.h"
#include "CustomEyeCenterDetector.h"
#include "LaplaceDetector.h"
#include "MinDetector.h"
#include "TemplateDetector.h"
#include "ThresholdDetector.h"
#include "Calibrator.h"
#include "Kalman.h"

#include "Timer.h"

using namespace cv;

class FaceDetector : public AppModule
{
private:
	CascadeClassifier faceClassifier;
	CascadeClassifier eyeLclassifier;
	CascadeClassifier eyeRclassifier;
	CascadeClassifier noseClassifier;
	CascadeClassifier mouthClassifier;
	CascadeClassifier eye2Lclassifier;
	CascadeClassifier eye2Rclassifier;
	
	vector<Rect> nose;
	vector<Rect> mouth;
	vector<Rect> faces;
	vector<Rect> eyesLeft;
	vector<Rect> eyesRight;

	HoughDetector houghDetector;
	MeansOfGradientsDetector meansOfGradientsDetector;
	LaplaceDetector laplaceDetector;
	MinDetector minDetector;
	CustomEyeCenterDetector cstDetector;
	TemplateDetector tmpDetector;
	ThresholdDetector treshDetector;

	Calibrator calibrator;
	Timer benchmark;

	Kalman kFilterLeye;
	Kalman kFilterReye;
	Kalman kFilterNose;
	Kalman KFilterMouth;
	Kalman kFilterRegionFace;
	Kalman kFilterRegionEyeLeft;
	Kalman kFilterRegionEyeRight;

	bool templAcquired;

	int faceMinSizeX;
	int faceMaxSizeX;
	int faceMinSizeY;
	int faceMaxSizeY;

	int faceScale;
	int faceFactor;

	int lEyeMinSizeX;
	int lEyeMaxSizeX;
	int lEyeMinSizeY;
	int lEyeMaxSizeY;

	int lEyeScale;
	int lEyeFactor;

	int rEyeMinSizeX;
	int rEyeMaxSizeX;
	int rEyeMinSizeY;
	int rEyeMaxSizeY;

	int rEyeScale;
	int rEyeFactor;

public:
	FaceDetector();
	~FaceDetector();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);

	void showSettingsWin();
};

