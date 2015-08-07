#ifndef __KALMAN_H__
#define __KALMAN_H__

#include "opencv2\opencv.hpp"

using namespace cv;

class Kalman {
private:
	KalmanFilter *kFilter;
public:
	Kalman();
	~Kalman();

	Point getPoint(Point ref);
	void setProcessNoiseCov(double s);
	void setMeasurementNoiseCov(double s);
	void setErrorCovPost(double s);
};

#endif