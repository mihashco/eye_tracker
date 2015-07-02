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

	//noice reduction
	blur(dst, dst, Size(3, 3));
	
	//Remove big lines for make thin contours
	dilate(dst, dst, this->erodeKernel);
	dilate(dst, dst, this->erodeKernel);

	Laplacian(dst, dst, dst.depth(), 3, 12.0, 3.0);

	//detect contours.
	vector<vector<Point> >contours;
	//findContours(dst, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	 
	

	imshow("Debug1", dst);
	return Point(-1, -1);
}