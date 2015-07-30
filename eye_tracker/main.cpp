#include "opencv2/opencv.hpp"
#include "EyeTracker.h"

using namespace cv;
using namespace std;


int main(int argc, char **argv)
{
	EyeTracker eyeTrackerApplication(argc, argv);

	eyeTrackerApplication.applicationModulesInit();
	eyeTrackerApplication.startApplicationLoop();
	eyeTrackerApplication.applicationModulesDeinit();

	return 0;
}