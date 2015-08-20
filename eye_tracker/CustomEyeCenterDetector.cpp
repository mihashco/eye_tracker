#include "CustomEyeCenterDetector.h"

CustomEyeCenterDetector::CustomEyeCenterDetector()
{
	/*namedWindow("Debug1", 0);
	namedWindow("Debug2", 0);
	namedWindow("Settings", 1);

	moveWindow("Debug1", 400, 0);
	moveWindow("Debug2", 400, 320);
	moveWindow("Settings", 400, 320 * 2);*/

	this->kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3));

	this->thresh = 220;
	this->threshMaxValue = 255;
	this->contourSize = 5;

	this->minArea = 0;
	this->maxArea = 200;
	
	/*createTrackbar("cannyThreshold1", "Settings", &this->cannyThreshold1, 2000, NULL, NULL);
	createTrackbar("cannyThreshold2", "Settings", &this->cannyThreshold2, 4000, NULL, NULL);
	createTrackbar("cannyApertureSize", "Settings", &this->cannyApertureSize, 10, NULL, NULL);

	createTrackbar("contourSize", "Settings", &this->contourSize, 50, NULL, NULL);
	createTrackbar("minArea", "Settings", &this->minArea, 200, NULL, NULL);
	createTrackbar("maxArea", "Settings", &this->maxArea, 200, NULL, NULL);
	createTrackbar("threshold", "Settings", &this->thresh, 255, NULL, NULL);*/
}

CustomEyeCenterDetector::~CustomEyeCenterDetector()
{

}

Point CustomEyeCenterDetector::detect(Mat &imgSource)
{
	//NOTE: Do not remove this!
	/*vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat edges;*/

	
	normalize(imgSource, imgSource, 0, 255, NORM_MINMAX, CV_8UC1);


	for (int i = 0; i < 2; i++)
	{
		erode(imgSource, imgSource, this->kernel);
		blur(imgSource, imgSource, Size(5, 5));
	}

	/*Mat gradX, gradY, grad, magnitudes, angles, cnvMagnitudes, cnvAngles;

	Sobel(imgSource, gradX, CV_32F, 0, 1, 3, 1, 1);
	Sobel(imgSource, gradY, CV_32F, 1, 0, 3, 1, 1);*/

	/*addWeighted(gradX, 0.5, gradY, 0.5, 0, grad);
	cartToPolar(gradX, gradY, magnitudes, angles);
*/
	//magnitudes.convertTo(cnvMagnitudes, CV_8U);

	//cnvMagnitudes = ~cnvMagnitudes;
	threshold(imgSource, imgSource, this->thresh, this->threshMaxValue, THRESH_BINARY);

	/*Canny(cnvMagnitudes, edges, (double) (cannyThreshold1/10), (double) (cannyThreshold2/10), 3, false);
	findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	vector<RotatedRect> minEllipse(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > this->contourSize)
		{
			double area = cv::contourArea(contours[i]);
			cv::Rect rect = cv::boundingRect(contours[i]);

			if (area >= minArea && area <= maxArea)
			{
				minEllipse[i] = fitEllipse(Mat(contours[i]));
			}
		}
	}
	
	Mat drawing = Mat::zeros(imgSource.size() , CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255,255,255);
		ellipse(imgSource, minEllipse[i], color, 1, 8);
	}


	imshow("Debug2", cnvMagnitudes);*/
	imshow("Debug2", imgSource);

	return Point(-1, -1);
}