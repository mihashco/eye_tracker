#include <string>
#include <vector>

#include "opencv2\opencv.hpp"
#include "AppModule.h"

using namespace cv;

class FrameAcquisitor : public AppModule
{
private:
	VideoCapture cap;
	Vector<String> imgPaths;
public:
	FrameAcquisitor();
	~FrameAcquisitor();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);

	void imgPathSet(String imgPath);
};