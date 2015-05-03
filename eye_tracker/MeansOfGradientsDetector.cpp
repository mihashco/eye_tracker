#include "MeansOfGradientsDetector.h"

MeansOfGradientsDetector::MeansOfGradientsDetector()
{
	this->weightDiv = 50.0;
	this->weightEnabled = true;
}

MeansOfGradientsDetector::~MeansOfGradientsDetector()
{
	//
}

void MeansOfGradientsDetector::showSettingsWin(void)
{
	namedWindow("MeansOfGradientsSettingsWin", 1);
//	createTrackbar("Method", "HoughDetectorSettingsWin", &this->weightDiv, NULL, NULL);
}

double MeansOfGradientsDetector::computeTreshold(Mat &magnitudes, double devFactor)
{
	Scalar stdMagGradient, meanMagGradient;
	meanStdDev(magnitudes, meanMagGradient, stdMagGradient);
	double dynamicTresholdTmp = stdMagGradient[0] / sqrt(magnitudes.rows * magnitudes.cols);
	return devFactor * dynamicTresholdTmp + meanMagGradient[0];
}

void MeansOfGradientsDetector::testCenter(int x, int y, Mat &img, double gX, double gY, Mat &out)
{
	for (int i = 0; i < out.rows; ++i)
	{
		double *rowPtrO = out.ptr<double>(i);
		uchar *rowPtrI = img.ptr<uchar>(i);

		for (int j = 0; j < out.cols; ++j)
		{
			if (x == i && y == j)
			{
				continue;
			}

			double dx = x - i;
			double dy = y - j;

			double mag = sqrt(dx*dx + dy * dy);
			dx = dx / mag;
			dy = dy / mag;

			double point = dx * gX + dy * gY;
			point = std::max(0.0, point);

			if (this->weightEnabled)
			{
				rowPtrO[j] = point * point * rowPtrI[j] / this->weightDiv;
			}
			else
			{
				rowPtrO[j] = point * point;
			}
		}
	}
}

Point MeansOfGradientsDetector::detect(Mat &img)
{
	Mat gradX, gradY;
	Mat grad, magnitudes, angles;
	Mat out, weight;
	Mat outSum = Mat::zeros(img.rows, img.cols, CV_64F);

	Point maxP;
	double maxVal, tresh, gradients;

	int scale = 1;
	int delta = 0;

	Sobel(img, gradX, CV_64F, 0, 1, 3, scale, delta);
	Sobel(img, gradY, CV_64F, 1, 0, 3, scale, delta);

	addWeighted(gradX, 0.5, gradY, 0.5, 0, grad);
	cartToPolar(gradX, gradY, magnitudes, angles);

	tresh = this->computeTreshold(magnitudes, this->weightDiv);

	for (int i = 0; i < img.rows; ++i)
	{
		double *rowPtrX = gradX.ptr<double>(i), *rowPtrY = gradY.ptr<double>(i);
		double *magPtr = magnitudes.ptr<double>(i);

		for (int j = 0; j < img.cols; ++j)
		{
			double valX = rowPtrX[j], valY = rowPtrY[j];
			double valMagnitude = magPtr[j];

			if (valMagnitude > tresh)
			{
				rowPtrX[j] = valX / valMagnitude;
				rowPtrY[j] = valY / valMagnitude;
			}
			else
			{
				rowPtrX[j] = 0.0;
				rowPtrY[j] = 0.0;
			}
		}
	}

	//blur source data
	GaussianBlur(img, weight, Size(3, 3), 0, 0);

	//invert image -> opencv function can be used for that operation
	for (int i = 0; i < weight.rows; ++i)
	{
		uchar *row = weight.ptr<uchar>(i);
		for (int j = 0; j < weight.cols; j++)
		{
			row[j] = (255 - row[j]);
		}
	}

	for (int i = 0; i < weight.rows; ++i)
	{
		double *rowPtrX = gradX.ptr<double>(i);
		double *rowPtrY = gradY.ptr<double>(i);

		for (int j = 0; j < weight.cols; ++j)
		{
			double valX = rowPtrX[j];
			double valY = rowPtrY[j];

			if (valX == 0.0 && valY == 0.0)
			{
				continue;
			}

			this->testCenter(i, j, weight, valX, valY, outSum);
		}
	}

	gradients = weight.rows * weight.cols;
	outSum.convertTo(out, CV_32F, 1.0 / gradients);
	minMaxLoc(out, NULL, &maxVal, NULL, &maxP);
	
	return maxP;
}