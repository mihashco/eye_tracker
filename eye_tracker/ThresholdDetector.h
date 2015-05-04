#ifndef __THRESHOLD_DETECTOR_H__
#define __THRESHOLD_DETECTOR_H__

#include "opencv2\opencv.hpp"

using namespace cv;

class ThresholdDetector {
private:
	int thresh;
	Mat kernel;
public:
	ThresholdDetector();
	~ThresholdDetector();

	Point detect(Mat &imgSrc);
};



#endif