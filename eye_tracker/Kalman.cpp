#include "Kalman.h"

Kalman::Kalman()
{
	kFilter = new KalmanFilter(4,2,0);

	kFilter->transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0,     0, 1, 0, 1,    0, 0, 1, 0,   0, 0, 0, 1);
	measurement = Mat_<float>(2, 1);
	measurement.setTo(Scalar(0));

	kFilter->statePre.at<float>(0) = 0;
	kFilter->statePre.at<float>(1) = 0;
	kFilter->statePre.at<float>(2) = 0;
	kFilter->statePre.at<float>(3) = 0;

	setIdentity(kFilter->measurementMatrix);
	setIdentity(kFilter->processNoiseCov, Scalar::all(1e-4));
	setIdentity(kFilter->measurementNoiseCov, Scalar::all(1e-1));
	setIdentity(kFilter->errorCovPost, Scalar::all(.1));
}

Kalman::~Kalman()
{
	delete kFilter;
}

Point Kalman::getPoint(Point ref)
{
	Mat prediction = kFilter->predict();
	Point predictPt(prediction.at<float>(0), prediction.at<float>(1));

	measurement(0) = ref.x;
	measurement(1) = ref.y;

	Point measPt(measurement(0), measurement(1));
	Mat estimated = kFilter->correct(measurement);

	return Point(estimated.at<float>(0), estimated.at<float>(1));
}

void Kalman::setErrorCovPost(double s)
{
	setIdentity(kFilter->processNoiseCov, Scalar::all(s));
}

void Kalman::setMeasurementNoiseCov(double s)
{
	setIdentity(kFilter->measurementNoiseCov, Scalar::all(s));
}

void Kalman::setProcessNoiseCov(double s)
{
	setIdentity(kFilter->errorCovPost, Scalar::all(s));
}