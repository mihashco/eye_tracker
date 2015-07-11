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

class ApplicationState {
public:
	/*Acquisitor settings*/
	AcqusitionSource acqSrc;
	int camID;
	Mat frameSrc;
	Mat frameDst;
	Mat frameGray;

	Mat frameR;
	Mat frameG;
	Mat frameB;

	int frameWidth;
	int frameHeight;

	/*FaceAndEyeDetector*/
	Mat face; /*best face candidate*/
	Rect faceRect;
	Mat lEye; /*best left eye candidate*/
	Mat rEye; /*best right eye candidate*/

	Point lEyeCenter;
	Point rEyeCenter;

	Point lEyeRelativeCenter;
	Point rEyeRelativeCenter;

	/*Head Pose estimation*/
	Point headCenter;
	double headYaw;
	double headPitch;
	double headRoll;

	/*CursorController*/
	ApplicationMode apMode;

	ApplicationState();
	~ApplicationState();
};




#endif