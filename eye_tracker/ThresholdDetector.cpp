#include "ThresholdDetector.h"


ThresholdDetector::ThresholdDetector()
{
	this->structuringElement = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));
}


ThresholdDetector::~ThresholdDetector()
{
}

Point ThresholdDetector::detect(Mat src)
{
	Point minLoc;
	double minVal;
	int bestIdx = 0;

	Mat img = src.clone();

	/*Preprocessing: Removing reflections*/
	blur(src, src, Size(5,5));
	blur(src, src, Size(5, 5));
	blur(src, src, Size(5, 5));
	blur(src, src, Size(5, 5));

	erode(img, img, this->structuringElement);

	resize(img, img, Size(86 * 4, 51 * 4));
	/*Detection: find minimum value for set the threshold */
	minMaxLoc(img, &minVal, NULL, &minLoc, NULL);

	/*Detection: increase the threshold value to improve the detection algorithm*/
	threshold(img, img, minVal + 1, 255, CV_THRESH_BINARY);

	imshow("DEBUG8", img);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<RotatedRect> minEllipse(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > 5)
		{
			minEllipse[i] = fitEllipse(Mat(contours[i]));
		}
	}

	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//drawContours(drawing, contours, i, Scalar(255,255,0), 1, 8, vector<Vec4i>(), 0, Point());
		// ellipse
		ellipse(drawing, minEllipse[i], Scalar(0,255,255), 2, 8);
	}

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	for (int i = 0; i < contours.size(); i++) {
		std::cout << "AREA :" << minEllipse[i].size.area() << std::endl;
		if (minEllipse[i].size.area() < 700.0) {
			bestIdx = i;
		}
	}
	return minEllipse[bestIdx].center;


}