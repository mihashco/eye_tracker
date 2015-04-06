/**
* This class is responsible for initialization of other program modules and start 
* loop of the application. It controls main loop and breaks it when receive valid
* keyboard signal
*/

#include "opencv2\opencv.hpp"
#include "FrameAcquisitor.h"
#include "FrameDetector.h"
#include "FramePreprocessor.h"

using namespace cv;

class EyeTracker
{
private:
	AppModule *modules[3];
	Mat srcFrame;
	Mat dstFrame;

	ushort modulesCount;
public:
	EyeTracker();
	~EyeTracker();

	void applicationModulesInit();
	void applicationModulesDeinit();

 	void startApplicationLoop();
};