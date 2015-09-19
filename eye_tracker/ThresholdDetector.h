#pragma once
#include "opencv2\opencv.hpp"

using namespace cv;

class ThresholdDetector
{
private:
	Mat structuringElement;
	SimpleBlobDetector smpBlobDetector;
public:
	ThresholdDetector();
	~ThresholdDetector();

	Point detect(Mat img);
};

