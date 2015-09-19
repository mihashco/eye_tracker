#include "MinDetector.h"

MinDetector::MinDetector()
{
	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3));
	this->kernel2 = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));
	//hDetector.showSettingsWin();
}

MinDetector::~MinDetector()
{
	//
}

Point MinDetector::detect(Mat src)
{
	Point minLoc;
	
	if (src.empty())
		return Point(-1, -1);

	normalize(src, src, 0, 255, NORM_MINMAX, CV_8UC1);
	equalizeHist(src, src);

	blur(src, src, Size(5, 5));
	erode(src, src, this->kernel);
	blur(src, src, Size(5, 5));
	resize(src, src, Size(86 * 3, 51 * 3));

	minMaxLoc(src, NULL, NULL, &minLoc, NULL);
	circle(src, minLoc, 10, Scalar(255, 255, 255), 2);
	
	imshow("MIN_DETECTOR_DEBUG", src);


	return minLoc;
}