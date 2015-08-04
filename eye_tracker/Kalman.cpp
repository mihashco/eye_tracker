#include "Kalman.h"

Kalman::Kalman()
{
	kFilter = new KalmanFilter(4,2,0);

	kFilter->transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	Mat_<float> measurement(2, 1);
	measurement.setTo(Scalar(0));

	kFilter->statePre.at<float>(0) = 1000;
	kFilter->statePre.at<float>(1) = 600;
	kFilter->statePre.at<float>(2) = 0;
	kFilter->statePre.at<float>(3) = 0;

	setIdentity(kFilter->measurementMatrix);
	setIdentity(kFilter->processNoiseCov, Scalar::all(.005));
	setIdentity(kFilter->measurementNoiseCov, Scalar::all(1e-1));
	setIdentity(kFilter->errorCovPost, Scalar::all(.1));
}

Kalman::~Kalman()
{
	delete kFilter;
}

Point Kalman::getPoint(Point ref)
{
	if (!kFilter) return Point(-1, -1);

	Mat prediction = kFilter->predict();
	Mat_<float> measurement(2, 1);

	measurement(0) = ref.x;
	measurement(1) = ref.y;

	Mat est = kFilter->correct(measurement);
	return Point(est.at<float>(0), est.at<float>(1));
}