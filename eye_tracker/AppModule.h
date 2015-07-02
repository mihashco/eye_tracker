#ifndef __APP_MODULE_H__
#define __APP_MODULE_H__ 

#include "opencv2\opencv.hpp"
#include "ApplicationState.h"

using namespace cv;

class AppModule
{
public:
	AppModule();
	~AppModule();

	virtual void moduleInit(void) {};
	virtual void moduleInit(ApplicationState &appState) {};
	
	virtual void moduleDeinit(void) {};
	virtual void moduleProcess(void) {};
	
	virtual void moduleProcess(Mat &inFrame, Mat &outFrame) {};
	virtual bool moduleProcess(ApplicationState &appState) {return true;};

	virtual void moduleFrameGet(Mat &frame) {};
};

#endif