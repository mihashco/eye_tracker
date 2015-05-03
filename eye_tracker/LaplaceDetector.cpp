#include "LaplaceDetector.h"

LaplaceDetector::LaplaceDetector()
{
	this->erodeKernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3, 3));

	this->blobDetector = new SimpleBlobDetector();
	this->blobDetector->create("SimpleBlob");
}

LaplaceDetector::~LaplaceDetector()
{

}

Point LaplaceDetector::detect(Mat &srcImg)
{
	Mat dst = srcImg.clone(); 
	dilate(dst, dst, this->erodeKernel);
	dilate(dst, dst, this->erodeKernel);

	Laplacian(dst, dst, dst.depth(), 3, 12.0, 1.0);

	//vector<KeyPoint> keyPoints;
	//this->blobDetector->detect(dst, keyPoints);

//	Mat out;
//	drawKeypoints(srcImg, keyPoints, out, Scalar(255, 255, 255));

//	imshow("Debug1", out);
	imshow("Debug2", dst);

	return Point(-1, -1);
}