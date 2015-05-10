#ifndef __APP_MODULE_H__
#define __APP_MODULE_H__ 

#include "opencv2/opencv.hpp"

using namespace cv;

class AppModule
{
public:
	AppModule();
	~AppModule();

	virtual void moduleInit(void) {};
	virtual void moduleDeinit(void) {};
	virtual void moduleProcess(void) {};
	virtual void moduleProcess(int x, int y) {};
	virtual void moduleProcess(Mat &inFrame, Mat &outFrame) {};

	virtual void moduleFrameGet(Mat &frame) {};
};

#endif