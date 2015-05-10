#ifndef __HOUGH_DETECTOR_H__
#define __HOUGH_DETECTOR_H__

#include "opencv2\opencv.hpp"
/*
This class uses the hough detector method from opencv and shows a window for that algorithm
*/

using namespace cv;

class HoughDetector {
private:
	int method;
	int dp;
	int minDist;
	int param1;
	int param2;
	int minRadious;
	int maxRadious;
public:
	HoughDetector();
	~HoughDetector();

	Point detect(Mat &img);
	void showSettingsWin(void);
};

#endif