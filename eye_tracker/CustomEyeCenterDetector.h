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

	SimpleBlobDetector blobDetector;

	int cannyThreshold1;
	int cannyThreshold2;
	int cannyApertureSize = 3;
	bool cannyL2grad = false;

	int contourSize;
	int minArea;
	int maxArea;

public:
	CustomEyeCenterDetector();
	~CustomEyeCenterDetector();

	Point detect(Mat &imgSource);
};

#endif