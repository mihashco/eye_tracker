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
	Mat heatMapTestImage;

	Kalman outKF;
	std::ofstream testResults;

	int currentTC;
	int currentRegion;

	bool winEyeVisible;
	bool winHeadVisible;

	int **regionsOutData;

public:
	GazeEstimator();
	~GazeEstimator();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);

	bool moduleProcessGazeTest(ApplicationState &appState, Point out);
	bool moduleProcessGazeDebug(ApplicationState &appState, Point out);
	bool moduleProcessGazeSixRegions(ApplicationState &appState, Point out);
	bool moduleProcessGazeSixRegionsTest(ApplicationState &appState, Point out);
	bool moduleProcessGazeHeatMap(ApplicationState &appState, Point out);

	void moduleDeinit(void);
};

#endif

