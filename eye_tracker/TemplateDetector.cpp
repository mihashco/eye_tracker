#include "TemplateDetector.h"


TemplateDetector::TemplateDetector()
{
	templ = Mat::zeros(30, 30, CV_8UC1);

	circle(templ, Point(15, 15), 7, Scalar(255), 14, 0);
}


TemplateDetector::~TemplateDetector()
{
}

Point TemplateDetector::detect(Mat &img, Mat temp) 
{ 
	if (!temp.empty()) {
		resize(img, img, Size(img.cols * 3, img.rows * 3));
		normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);
		matchTemplate(img, temp, img, CV_TM_CCOEFF);
		normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("Debug4", img);

		Point minLoc;
		Point maxLoc;

		minMaxLoc(img, NULL, NULL, &minLoc, &maxLoc);
		return minLoc;
	}

	return Point(-1, -1);
}