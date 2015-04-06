#include "AppModule.h"

class FramePreprocessor : public AppModule
{
public:
	FramePreprocessor();
	~FramePreprocessor();

	void moduleInit();
	void moduleDeinit();
	void moduleProcess(Mat &inFrame, Mat &outFrame);
};

