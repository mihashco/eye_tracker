#ifndef __LAPLACE_DETECTOR_H__
#define __LAPLACE_DETECTOR_H__

#include "opencv2\opencv.hpp"
using namespace cv;

class LaplaceDetector {
private:
	int ddepth;
	int kSize;
	double scale;
	double detla;
	Mat erodeKernel;

	SimpleBlobDetector *blobDetector;
public:
	LaplaceDetector();
	~LaplaceDetector();
	Point detect(Mat &srcImg);
};

#endif