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

	int frameWidth;
	int frameHeight;

	/*FaceFeatures*/
	Mat face; /*best face candidate*/
	Mat lEye; /*best left eye candidate*/
	Mat rEye; /*best right eye candidate*/

	/*This variables are used for calculate head parameters*/
	Rect faceRect;
	Rect noseRect;
	Rect mouthRect;

	/*PUPIL CENTER*/
	Point lEyeCenter;
	Point rEyeCenter;
	Point lEyeRelativeCenter;
	Point rEyeRelativeCenter;
	
	Point eyeAproxCenter;

	/*Nose*/
	Point noseCenter;
	Point noseAproxCenter;

	/*Mouth points*/
	Point lmouthCorner;
	Point rmouthCorner;
	Point mouthCenter;
	Point mouthAproxCenter;

	/*Eyes position*/
	Point leftEyeLcorner;
	Point leftEyeRcorner;
	Point rightEyeLcorner;
	Point rightEyeRcorner;

	/*Head Pose estimation*/
	Point headCenter;
	Point headAproxCenter; /*Estimated by Kalman Filter*/
	
	int headOX;
	int headOY;
	int headOZ;

	/*DATA AFTER CALIBRATION*/
	Point headForScreenCenter;
	Point eyeForScreenCenter;

	int oyForScreenCenter;
	int oxForScreenCenter;

	/*CursorController*/
	ApplicationMode apMode;

	ApplicationState();
	~ApplicationState();
};




#endif