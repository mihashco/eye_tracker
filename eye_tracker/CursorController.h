#ifndef __CURSOR_CONTROLLER_H__
#define __CURSOR_CONTROLLER_H__

#include "opencv2\opencv.hpp"

#include <vector>
#include <algorithm>

#include "AppModule.h"

using namespace cv;

class CursorController : public AppModule {
private:
	Mat canvas;
	Point center;
	Point leftTop;
	Point leftBottom;
	Point rightTop;
	Point rightBottom;

	bool isCalibrated;
	bool isCalibrationDataCollected;

	static const int dCounter = 30;

	int cX, cY;
	int lX, rX, dlX, drX;
	int tY, bY, dtY, dbY;

	int scrWidth = 640;
	int scrHeight = 480;
	
	std::vector<Point> cPoints;
	std::vector<Point> ltPoints;
	std::vector<Point> lbPoints;
	std::vector<Point> rtPoints;
	std::vector<Point> rbPoints;

	/*6 regions mode calibration data*/
	std::vector<int> leftX;
	std::vector<int> centerX;
	std::vector<int> centerY;
	std::vector<int> rightX;
	std::vector<int> topY;
	std::vector<int> bottomY;

	std::vector<Point> sortedLx;
	std::vector<Point> sortedRx;
	std::vector<Point> sortedCx;
	std::vector<Point> sortedCy;
	std::vector<Point> sortedTy;
	std::vector<Point> sortedBy;

	double mCenterX;
	double mCenterY;
	double mLeftX;
	double mRightX;
	double mTopY;
	double mBottomY;

	bool processCalibration(ApplicationState &appState);
	bool collectCalibrationData(ApplicationState &appState);
	void computeCallibrationData(ApplicationState &appState);

	bool processModeGazeTracker(ApplicationState &appState);
	bool processMode6Regions(ApplicationState &appState);

public:
	CursorController();
	~CursorController();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
};


#endif