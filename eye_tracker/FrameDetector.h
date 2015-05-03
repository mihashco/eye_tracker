#include "opencv2\opencv.hpp"
#include "AppModule.h"

#include "HoughDetector.h"
#include "MeansOfGradientsDetector.h"
#include "CustomEyeCenterDetector.h"
#include "LaplaceDetector.h"
#include "Timer.h"

using namespace cv;

class FrameDetector : public AppModule
{
private:
	CascadeClassifier eyeCalssifier;
	CascadeClassifier faceClassifier;

	vector<Rect> faces;
	vector<Rect> eyes;

	Mat faceRoi;
	Mat eye1Roi;
	Mat eye2Roi;

	HoughDetector houghEyeCenterDetector;
	MeansOfGradientsDetector meansOfGradientsDetector;
	CustomEyeCenterDetector cstDetector;
	LaplaceDetector lDetector;

	bool testMode;

	Timer benchmark;
	
public:
	FrameDetector();
	~FrameDetector();

	void moduleInit(void);
	void moduleDeinit(void);
	void moduleProcess(Mat &srcFrame, Mat &dstFrame);
	void printEyes(void);

	void moduleSetTestModeState(bool mode);
	bool isTestModeOn();
};

