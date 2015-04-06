#include "opencv2\opencv.hpp"
#include "AppModule.h"

using namespace cv;

class FrameDetector : public AppModule
{
private:
	CascadeClassifier eyeCalssifier;
	CascadeClassifier faceClassifier;

	vector<Rect> faces;
	vector<Rect> eyes;
	
public:
	FrameDetector();
	~FrameDetector();

	void moduleInit(void);
	void moduleDeinit(void);
	void moduleProcess(Mat &srcFrame, Mat &dstFrame);
};

