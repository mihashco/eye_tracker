/**
* This class is responsible for initialization of other program modules and start 
* loop of the application. It controls main loop and breaks it when receive valid
* keyboard signal
*/

#include "opencv2\opencv.hpp"

#include "FrameAcquisitor.h"
#include "FaceAndEyeDetector.h"
#include "FramePreprocessor.h"
#include "ApplicationState.h"

using namespace cv;

class EyeTracker
{
private:
	ApplicationState appState;

	AppModule *modules[10];
	ushort modulesCount;

public:
	EyeTracker();
	~EyeTracker();

	void applicationModulesInit();
	void applicationModulesDeinit();
 	void startApplicationLoop();
};