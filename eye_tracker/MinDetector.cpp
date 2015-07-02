#include "MinDetector.h"

MinDetector::MinDetector()
{
	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3));
	hDetector.showSettingsWin();
}

MinDetector::~MinDetector()
{
	//
}

Point MinDetector::detect(Mat src, Point &relativeCenter, Point rel, int &dx, int &dy)
{
	Point minLoc, maxLoc;
	double minVal, maxVal;

	if (src.empty())
		return Point(-1, -1);
	
	minMaxLoc(src, &minVal, &maxVal, &minLoc, &maxLoc);
	circle(src, minLoc, 5, Scalar(255, 255, 255), 3);

	relativeCenter = minLoc;
	dx = abs(rel.x - minLoc.x);
	dy = abs(rel.y - minLoc.y);

	return minLoc;
}