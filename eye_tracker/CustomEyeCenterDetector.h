#ifndef __CUSTOM_EYE_CENTER_DETECTOR__
#define __CUSTOM_EYE_CENTER_DETECTOR__

#include "opencv2\opencv.hpp"

using namespace cv;

class CustomEyeCenterDetector {
private:
	Mat kernel;
	Mat erodeOutput;

	int thresh;
	int threshMaxValue;

public:
	CustomEyeCenterDetector();
	~CustomEyeCenterDetector();

	Point detect(Mat &imgSource);
};

#endif