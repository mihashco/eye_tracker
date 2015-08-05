/**
* This class is responsible for initialization of other program modules and start 
* loop of the application. It controls main loop and breaks it when receive valid
* keyboard signal
*/

#include "opencv2\opencv.hpp"

#include "FrameAcquisitor.h"
#include "FaceDetector.h"
#include "FramePreprocessor.h"
#include "ApplicationState.h"
#include "HeadDetector.h"
#include "GazeEstimator.h"

using namespace cv;

class EyeTracker
{
private:
	ApplicationState appState;
	AppModule *modules[10];
	ushort modulesCount;

	int winId;

public:
	EyeTracker(int argc, char **argv);
	~EyeTracker();

	void applicationModulesInit(void);
	void applicationModulesDeinit(void);
 	void startApplicationLoop(void);

	void draw();
};