#include "opencv2/opencv.hpp"

#include "EyeTracker.h"

using namespace cv;
using namespace std;


int main(int, char**)
{
	EyeTracker eyeTrackerApplication;

	eyeTrackerApplication.applicationModulesInit();
	eyeTrackerApplication.startApplicationLoop();

	return 0;
}