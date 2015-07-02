#include "opencv2\opencv.hpp"
#include "opencv2\contrib\detection_based_tracker.hpp"
#include "AppModule.h"

#include "HoughDetector.h"
#include "MeansOfGradientsDetector.h"
#include "CustomEyeCenterDetector.h"
#include "LaplaceDetector.h"
#include "MinDetector.h"
#include "CursorController.h"

#include "Timer.h"

using namespace cv;

class FaceAndEyeDetector : public AppModule
{
private:
	CascadeClassifier faceClassifier;
	CascadeClassifier eyeLclassifier;
	CascadeClassifier eyeRclassifier;

	vector<Rect> faces;
	vector<Rect> eyesLeft;
	vector<Rect> eyesRight;

	HoughDetector houghDetector;
	MeansOfGradientsDetector meansOfGradientsDetector;
	LaplaceDetector laplaceDetector;
	MinDetector minDetector;

	CursorController crsController;

	Timer benchmark;
	Mat lEye;
	Mat rEye;
public:
	FaceAndEyeDetector();
	~FaceAndEyeDetector();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
};

