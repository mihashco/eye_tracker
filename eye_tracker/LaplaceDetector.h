#ifndef __LAPLACE_DETECTOR_H__
#define __LAPLACE_DETECTOR_H__

#include "opencv2\opencv.hpp"

using namespace cv;

class LaplaceDetector {
private:
	Mat erodeKernel;
public:
	LaplaceDetector();
	~LaplaceDetector();

	Point detect(Mat &imgSrc);
};

#endif