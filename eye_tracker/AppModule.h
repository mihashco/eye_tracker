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

	virtual void moduleInit(ApplicationState &appState) {};
	virtual void moduleDeinit(void) {};
	virtual bool moduleProcess(ApplicationState &appState) {return true;};

};

#endif