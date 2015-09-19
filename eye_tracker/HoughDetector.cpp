#include "HoughDetector.h"

HoughDetector::HoughDetector()
{
	this->method = CV_HOUGH_GRADIENT;
	this->dp = 1;
	this->minDist = 62;
	this->param1 = 200;
	this->param2 = 0;
	this->minRadious = 12;
	this->maxRadious = 15;

	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3,3));
}

HoughDetector::~HoughDetector()
{
	//
}

void HoughDetector::showSettingsWin(void)
{
	namedWindow("HoughDetectorSettingsWin", 1);
	createTrackbar("Method", "HoughDetectorSettingsWin", &this->method, 3);
	createTrackbar("Dp", "HoughDetectorSettingsWin", &this->dp, 20);
	createTrackbar("MinDist", "HoughDetectorSettingsWin", &this->minDist, 1000);
	createTrackbar("Param1", "HoughDetectorSettingsWin", &this->param1, 1000);
	createTrackbar("Param2", "HoughDetectorSettingsWin", &this->param2, 1000);
	createTrackbar("minRadious", "HoughDetectorSettingsWin", &this->minRadious, 300);
	createTrackbar("maxRadious", "HoughDetectorSettingsWin", &this->maxRadious, 300);
}

Point HoughDetector::detect(Mat &img)
{
	vector<Vec3f> circles;
	vector<vector<int> > retVal;
	normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);
	std::stringstream ss;

	if (img.empty())
		return Point(-1,-1);

	equalizeHist(img, img);
	//erode(img, img, this->kernel);
	//dilate(img, img, this->kernel);
	//resize(img, img, Size(img.size().width * 6, img.size().height * 6));

	HoughCircles(img, circles, CV_HOUGH_GRADIENT, this->dp+1, this->minDist+1, this->param1+1, this->param2+1, this->minRadious+1, this->maxRadious+1);

	float x, y, r;
	static int file_name_n = 0;

	for (int i = 0; i < circles.size(); i++)
	{
		if (circles[i].rows == 3)
		{
			x = circles[i][0];
			y = circles[i][1];
			r = circles[i][2];

			//circle(img, Point(x, y), r, Scalar(255, 255, 255));

			vector<int> circleData;
			circleData.push_back(x);
			circleData.push_back(y);
			circleData.push_back(r);
		}
	}

	//ss << "test_" << file_name_n++ << ".jpg";

	//imwrite(ss.str(), img);

	/*Check the best candidate for the eye center. It should be the darkest circle -> pupil is the biggest dark spot on the picture*/
	/*for (int i = 0; i < retVal.size(); i++) {
		vector<int> circleData = retVal[i];
		Mat tmp = Mat::zeros(Size(circleData[0], circleData[1]), CV_8U);
	}*/
	/*Get the rectangle from the picture*/

	/*if (retVal.size() > 0) {
		vector<int> ret = retVal[0];
		return Point(ret[0], ret[1]);
	} else
		return Point(-1, -1);*/

	if (circles.size() > 0)
		return Point(circles[0][0], circles[0][1]);
	else
		return Point(-1, -1);
}