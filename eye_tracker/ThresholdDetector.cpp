#include "ThresholdDetector.h"


ThresholdDetector::ThresholdDetector()
{
	this->structuringElement = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));
}


ThresholdDetector::~ThresholdDetector()
{
}

Point ThresholdDetector::detect(Mat &img)
{
	Point minLoc;
	double minVal;

	resize(img, img, Size(img.cols * 4, img.rows * 4));

	/*Preprocessing: Normalization*/
	normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);

	/*Preprocessing: Equalize histogram*/
	equalizeHist(img, img);

	/*Preprocessing: Removing reflections*/
	erode(img, img, this->structuringElement);

	/*Detection: find minimum value for set the threshold */
	minMaxLoc(img, &minVal, NULL, &minLoc, NULL);

	/*Detection: increase the threshold value to improve the detection algorithm*/
	threshold(img, img, minVal + 1, 255, CV_THRESH_BINARY);

	imshow("Debug_Threshold_Detector", img);

	return Point(-1, -1);
}