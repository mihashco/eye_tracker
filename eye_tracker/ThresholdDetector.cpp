#include "ThresholdDetector.h"

ThresholdDetector::ThresholdDetector()
{
	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3));
	this->thresh = 50;
	createTrackbar("ThresholdVal", "Debug1", &this->thresh, 255, NULL, NULL);
}

ThresholdDetector::~ThresholdDetector()
{
	//
}

Point ThresholdDetector::detect(Mat &imgSrc)
{
	Mat dst = imgSrc.clone();

	dilate(dst, dst, this->kernel);
	threshold(dst, dst, this->thresh, 255, THRESH_BINARY);

	imshow("Debug1", dst);
	imshow("Debug2", imgSrc);

	return Point(-1, -1);
}