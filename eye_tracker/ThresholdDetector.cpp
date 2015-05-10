#include "ThresholdDetector.h"

ThresholdDetector::ThresholdDetector()
{
	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));
	this->thresh = 20;
	createTrackbar("ThresholdVal", "Debug1", &this->thresh, 255, NULL, NULL);
}

ThresholdDetector::~ThresholdDetector()
{
	//
}

Point ThresholdDetector::detect(Mat &imgSrc)
{
	Mat dst = imgSrc.clone();
	dst = ~dst;

	threshold(dst, dst, this->thresh, 255, THRESH_BINARY);
	erode(dst, dst, this->kernel);

	vector<KeyPoint> keypoints;
	this->blobDetector.detect(dst, keypoints);

	//drawKeypoints(imgSrc, keypoints);
	
	imshow("Debug1", dst);
	imshow("Debug2", imgSrc);

	return Point(-1, -1);
}