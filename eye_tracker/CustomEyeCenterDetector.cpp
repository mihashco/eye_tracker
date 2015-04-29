#include "CustomEyeCenterDetector.h"

CustomEyeCenterDetector::CustomEyeCenterDetector()
{
	namedWindow("Debug1", 0);
	namedWindow("Debug2", 0);
	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));

	this->thresh = 220;
	this->threshMaxValue = 255;

	createTrackbar("binLevelLow", "Debug1", &this->thresh, 255, NULL, NULL);
	createTrackbar("binLevelHigh", "Debug1", &this->threshMaxValue, 255, NULL, NULL);
}

CustomEyeCenterDetector::~CustomEyeCenterDetector()
{

}

Point CustomEyeCenterDetector::detect(Mat &imgSource)
{
	Mat dst;
	dst = ~imgSource;

	threshold(dst, dst, this->thresh, this->threshMaxValue, THRESH_BINARY);
	std::vector<std::vector<cv::Point> > contours;

	findContours(dst.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(dst, contours, -1, CV_RGB(255, 255, 255), -1);

	for (int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours[i]);
		cv::Rect rect = cv::boundingRect(contours[i]);
		int radius = rect.width / 2;

		// If contour is big enough and has round shape
		// Then it is the pupil
		if (area >= 30 &&
			std::abs(1 - ((double)rect.width / (double)rect.height)) <= 0.2 &&
			std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
		{
			cv::circle(dst, cv::Point(rect.x + radius, rect.y + radius), radius, CV_RGB(255, 0, 0), 2);
		}
	}

	imshow("Debug1", dst);
	return Point(-1, -1);
}