#ifndef __APPLICATION_STATE_H__
#define __APPLICATION_STATE_H__

#include "opencv2\opencv.hpp"


using namespace cv;

enum AcqusitionSource{
	ACQUISITION_VIDEO_CAP = 0,
	ACQUISITION_IMAGES,
	ACQUISITION_TEST_VIDEO,
};

enum ApplicationMode {
	APPLICATION_MODE_6_REGIONS = 0,
	APPLICATION_MODE_GAZE_TRACKER,
};

enum HeadAngleReference {
	HEAD_NOSE = 0,
	HEAD_MOUTH
};

enum UsedEye {
	EYE_LEFT = 0,
	EYE_RIGHT
};

enum GazeMode {
	GAZE_TEST = 0,
	GAZE_HEAT_MAP,
	GAZE_CURSOR_CONTROLLER,
	GAZE_SIZ_REGIONS,
	GAZE_DEBUG,
};

class ApplicationState {
public:
	AcqusitionSource acqSrc; /*Acqusition source*/
	HeadAngleReference headAngleRef; /*Part of face used for head angle calculation*/
	UsedEye usedEye; /*Eye used for gaze estimation*/
	ApplicationMode apMode; /*Application mode*/
	GazeMode gzMode;

	bool isCalibrated; /*Calibration status*/

	int measureTime; /*Time of the gaze estimation*/

	int camID;
	Mat frameSrc;
	Mat frameDst;
	Mat frameGray;
	int frameWidth;
	int frameHeight;

	/*FaceFeatures*/
	Mat faceMat; /*best face candidate*/
	Mat lEyeMat; /*best left eye candidate*/
	Mat rEyeMat; /*best right eye candidate*/

	/*This variables are used for calculate head parameters*/
	Rect faceRect;
	Rect noseRect;
	Rect mouthRect;

	/*PUPIL CENTER*/
	Point lEyeCenterPoint;
	Point rEyeCenterPoint;
	Point lEyeRelativeCenterPoint;
	Point rEyeRelativeCenterPoint;
	Point eyeAproxCenterPoint;

	Point eyeForScreenCenterPoint;

	/*FOR GAZE POINTER CALCULATION*/
	double eyeDeltaX;
	double eyeDeltaY;

	/*FROM CALIBRATOR*/
	Point eyeMeanCenterPoint;
	double leftFactor;
	double rightFactor;

	/*Nose*/
	Point noseCenterPoint;
	Point noseAproxCenterPoint;

	/*Mouth points*/
	Point lmouthCornerPoint;
	Point rmouthCornerPoint;
	Point mouthCenterPoint;
	Point mouthAproxCenterPoint;

	/*Head Pose estimation*/
	Point headCenterPoint;
	Point headAproxCenterPoint;

	double initHeadOX;
	double initHeadOY;
	int headOX;
	int headOY;
	double deltaHeadOX;
	double deltaHeadOY;

	/*OUTPUT*/
	Point gazePoint;
	Mat heatMapMat;

	ApplicationState();
	~ApplicationState();
};




#endif