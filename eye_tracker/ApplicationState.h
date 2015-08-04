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

class ApplicationState {
public:
	AcqusitionSource acqSrc; /*Acqusition source*/
	HeadAngleReference headAngleRef; /*Part of face used for head angle calculation*/
	UsedEye usedEye; /*Eye used for gaze estimation*/
	ApplicationMode apMode; /*Application mode*/
	
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
	Point eyeMeanCenterPoint;
	Point eyeForScreenCenterPoint;

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
	Point headAproxCenterPoint; /*Estimated by Kalman Filter*/
	Point headForScreenCenterPoint;
	int oyForScreenCenter;
	int oxForScreenCenter;
	int headOX;
	int headOY;

	/*OUTPUT*/
	Point gazePoint;
	Point aproxGazePoint;
	Mat heatMapMat;

	ApplicationState();
	~ApplicationState();
};




#endif