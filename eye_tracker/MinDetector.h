#ifndef __MIN_DETECTOR_H__
#define __MIN_DETECTOR_H__

#include "opencv2\opencv.hpp"

#include "HoughDetector.h"

using namespace cv;

class MinDetector
{
private:
	Mat mResult;
	Mat kernel;

	HoughDetector hDetector;

	static const int TM_SQDIFF = 0;
	static const int TM_SQDIFF_NORMED = 1;
	static const int TM_CCOEFF = 2;
	static const int TM_CCOEFF_NORMED = 3;
	static const int TM_CCORR = 4;
	static const int TM_CCORR_NROMED = 5;

	int learnFrames = 0;

public:
	MinDetector();
	~MinDetector();

	Point detect(Mat src, Point &relativeCenter, Point rel, int &distanceX, int &distanceY);
};


#endif