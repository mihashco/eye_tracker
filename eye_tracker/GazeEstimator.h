#ifndef __GAZE_ESTIMATOR_H__
#define __GAZE_ESTIMATOR_H__

#include "AppModule.h"
#include "opencv2\opencv.hpp"

#include "Kalman.h"
#include <fstream>

using namespace cv;

class GazeEstimator : public AppModule
{
private:
	Mat heatMap;
	Mat gazeCanvas;
	Mat rotation;

	Kalman outKF;
	std::ofstream testResults;

public:
	GazeEstimator();
	~GazeEstimator();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
};

#endif

