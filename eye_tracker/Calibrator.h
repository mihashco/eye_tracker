#ifndef __CALIBRATOR_H__
#define __CALIBRATOR_H__

#include <vector>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Calibrator {
private:
	Point center;
	
	//For calibrate x axis
	Point left;
	Point right;

	//For calibrate y axis
	Point top;
	Point bottom;

	vector<Point > centerV;
	vector<Point > leftV;
	vector<Point > rightV;
	vector<Point > bottomV;
	vector<Point > topV;

	double mvX;
	double mvY;

	void computeDeltaXY(void);

public:
	Calibrator();
	~Calibrator();

	bool moduleProcess(int x, int y);
	void moveCursor(int x, int y);
};


#endif