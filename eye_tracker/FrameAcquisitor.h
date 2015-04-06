#include "opencv2\opencv.hpp"
#include "AppModule.h"

using namespace cv;

class FrameAcquisitor : public AppModule
{
private:
	VideoCapture cap;
public:
	FrameAcquisitor();
	~FrameAcquisitor();

	void moduleInit(void);
	void moduleDeinit(void);
	void moduleProcess(Mat &srcFrame, Mat &dstFrame);
};
