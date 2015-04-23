#ifndef __MEANS_OF_GRADIENTS_DETECTOR_H__
#define __MEANS_OF_GRADIENTS_DETECTOR_H__

#include "opencv2\opencv.hpp"

using namespace cv;

class MeansOfGradientsDetector {
private:
	bool weightEnabled;
	double weightDiv;

	double computeTreshold(Mat &magnitudes, double devFactor);
	void testCenter(int x, int y, Mat &img, double gX, double gY, Mat &out);

	double valXBound;
	double valYBound;

public:
	MeansOfGradientsDetector();
	~MeansOfGradientsDetector();

	Point detect(Mat &img);
	void showSettingsWin(void);
};

#endif
