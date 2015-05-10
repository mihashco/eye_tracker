#include "HoughDetector.h"
#include <vector>

using namespace std;

HoughDetector::HoughDetector()
{
	this->method = CV_HOUGH_GRADIENT;
	this->dp = 3;
	this->minDist = 150;
	this->param1 = 50;
	this->param2 = 110;
	this->minRadious = 40;
	this->maxRadious = 45;
}

HoughDetector::~HoughDetector()
{
	//
}

void HoughDetector::showSettingsWin(void)
{
	namedWindow("HoughDetectorSettingsWin", 0);
	createTrackbar("Method", "HoughDetectorSettingsWin", &this->method, 3);
	createTrackbar("Dp", "HoughDetectorSettingsWin", &this->dp, 20);
	createTrackbar("MinDist", "HoughDetectorSettingsWin", &this->minDist, 300);
	createTrackbar("Param1", "HoughDetectorSettingsWin", &this->param1, 1000);
	createTrackbar("Param2", "HoughDetectorSettingsWin", &this->param2, 1000);
	createTrackbar("minRadious", "HoughDetectorSettingsWin", &this->minRadious, 300);
	createTrackbar("maxRadious", "HoughDetectorSettingsWin", &this->maxRadious, 300);
}

Point HoughDetector::detect(Mat &img)
{
	vector<Vec3f> circles;
	vector<Point> retVal;

	resize(img, img, Size(200, 200));

	GaussianBlur(img, img, Size(3, 3), 2, 2);
	equalizeHist(img, img);

	HoughCircles(img, circles, CV_HOUGH_GRADIENT, this->dp+1, this->minDist+1, this->param1+1, this->param2+1, this->minRadious+1, this->maxRadious+1);

	float x, y, r;

	for (int i = 0; i < circles.size(); i++)
	{
		if (circles[i].rows == 3)
		{
			x = circles[i][0];
			y = circles[i][1];
			r = circles[i][2];

			retVal.push_back(Point(x, y));
			circle(img, Point(x, y), r, Scalar(255, 255, 255), 1);
		}
	}

	imshow("Debug1", img);

	if (retVal.size() >= 1)
		return retVal[0];
	else
		return Point(-1, -1);
}