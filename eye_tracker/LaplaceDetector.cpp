#include "LaplaceDetector.h"

LaplaceDetector::LaplaceDetector()
{
	this->erodeKernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3));
}

LaplaceDetector::~LaplaceDetector()
{
	//
}

Point LaplaceDetector::detect(Mat &imgSrc)
{
	if (imgSrc.empty())
		return Point(-1, -1);

	Mat dst = imgSrc.clone();
	dst = ~dst;

	/*dilate(dst, dst, this->erodeKernel);
	dilate(dst, dst, this->erodeKernel);
	dilate(dst, dst, this->erodeKernel);*/

	Laplacian(dst, dst, imgSrc.depth(), 25, 7.0, 2.0);

	imshow("Debug1", dst);
	imshow("Debug2", imgSrc);

	return Point(- 1, -1);
}