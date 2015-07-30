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
	CursorController crsController;

	Timer benchmark;
public:
	FaceDetector();
	~FaceDetector();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
};

