#ifndef __HEAD_DETECTOR_H__
#define __HEAD_DETECTOR_H__

#include "AppModule.h"

class HeadDetector : public AppModule
{
private:
	double oxAngle;
	double oyAngle;
	double ozAngle;

	KalmanFilter *headCenterKF;
	KalmanFilter *noseCenterKF;
	KalmanFilter *mouthCenterKF;
	KalmanFilter *eyeCenterKF;
	KalmanFilter *gazeOutKF;

	Mat rotation;
	Mat visited;
	Mat checkedImage;

	int measureTime;

public:
	HeadDetector();
	~HeadDetector();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
	void kalmanFilterInit(KalmanFilter *kf);
	void kalmanFilterGetAproximatedPoint(KalmanFilter *kf, Point refPoint, Point &outPoint);
	void computeHeatMap(ApplicationState &appState);
};

#endif

