#ifndef __TEMPLATE_DETECTOR_H__
#define __TEMPLATE_DETECTOR_H__

#include "opencv2\opencv.hpp"

using namespace cv;

class TemplateDetector
{
private:
	Mat templ;
public:
	TemplateDetector();
	~TemplateDetector();

	Point detect(Mat &img, Mat temp);
};

#endif
