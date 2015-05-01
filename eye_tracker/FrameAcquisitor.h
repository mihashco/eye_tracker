#include <string>
#include <vector>

#include "opencv2\opencv.hpp"
#include "AppModule.h"

enum AcqusitionSource{
	ACQUISITION_VIDEO_CAP = 0,
	ACQUISITION_IMAGES,
};

using namespace cv;

class FrameAcquisitor : public AppModule
{
private:
	VideoCapture cap;
	AcqusitionSource source;
	Vector<String> imgPaths;
public:
	FrameAcquisitor();
	~FrameAcquisitor();

	void moduleInit(void);
	void moduleDeinit(void);
	void moduleProcess(Mat &srcFrame, Mat &dstFrame);

	void imgPathSet(String imgPath);
	void acquistionSourceSet(AcqusitionSource src);
};