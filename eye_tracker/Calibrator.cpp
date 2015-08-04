#include "Calibrator.h"


bool _internal_sort_points(Point p1, Point p2)
{
	if (p1.y > p2.y)
		return true;
	else
		return false;
}

Calibrator::Calibrator() 
{
	namedWindow("Canvas", 0);
	isCalibrationDataCollected = false;
	isCalibrated = false;

	cX = 0; cY = 0; lX = 0; rX = 0; tY = 0; bY = 0;
	canvas = Mat::zeros(Size(scrWidth, scrHeight), CV_8U);
}

Calibrator::~Calibrator()
{

}

void Calibrator::moduleInit(ApplicationState &appState)
{
	appState.apMode = APPLICATION_MODE_6_REGIONS;
}

bool Calibrator::moduleProcess(ApplicationState &appState)
{
	Point eyeCenter = appState.lEyeCenterPoint;

	if (isCalibrationDataCollected == false) {
		
		if (!collectCalibrationData(appState))
			return true;

		isCalibrationDataCollected = true;
	}

	if (isCalibrated == false) {
		computeCallibrationData(appState);
		isCalibrated = true;
		appState.isCalibrated = true;
	} else {
		if (appState.apMode == APPLICATION_MODE_6_REGIONS) {
			processMode6Regions(appState);
		}
		else if (appState.apMode == APPLICATION_MODE_GAZE_TRACKER) {
			processModeGazeTracker(appState);
		}
	}

	imshow("Canvas", canvas);
	return true;
}

void Calibrator::moduleDeinit(void)
{

}

bool Calibrator::processModeGazeTracker(ApplicationState &appState)
{
	Point eyeCenter = appState.lEyeRelativeCenterPoint;
	Point cursor;

	canvas = Mat::zeros(Size(scrWidth, scrHeight), CV_8U);

	int nx = 0, ny = 0;
	int dx = 1, dy = 1;

	if (eyeCenter.x - cX <= 0 && eyeCenter.y - cY > 0) {
		rectangle(canvas, Rect(scrWidth / 2, scrHeight / 2, scrWidth/2, scrHeight/2), Scalar(255, 255, 255), 5);

		if (dlX != 0)
			dx = (scrWidth / 2) / dlX;
		if (dbY != 0)
			dy = (scrHeight / 2) / dbY;

		nx = scrWidth / 2 + abs(cX - eyeCenter.x) * dx;
		ny = scrHeight / 2 + abs(cY - eyeCenter.y) * dy;
	} else if (eyeCenter.x - cX <= 0 && eyeCenter.y - cY <= 0) {
		rectangle(canvas, Rect(scrWidth / 2, 0, scrWidth / 2, scrHeight / 2), Scalar(255, 255, 255), 5);

		if (dlX != 0)
			dx = (scrWidth / 2) / dlX;
		if(dtY != 0 )
			dy = (scrHeight / 2) / dtY;

		nx = scrWidth / 2 + abs(cX - eyeCenter.x) * dx;
		ny = scrHeight / 2  - abs(cY - eyeCenter.y) * dy;
	} else if (eyeCenter.x - cX > 0 && eyeCenter.y - cY <= 0) {
		rectangle(canvas, Rect(0, 0, scrWidth / 2, scrHeight / 2), Scalar(255, 255, 255), 5);
		
		if (drX != 0)
			dx = (scrWidth / 2) / drX;
		if(dtY != 0)
			dy = (scrHeight / 2) / dtY;

		nx = scrWidth / 2 - abs(cX - eyeCenter.x) * dx;
		ny = scrHeight / 2 - abs(cY - eyeCenter.y) * dy;
	} else if (eyeCenter.x - cX > 0 && eyeCenter.y - cY > 0) {
		rectangle(canvas, Rect(0, scrHeight / 2, scrWidth / 2, scrHeight / 2), Scalar(255, 255, 255), 5);
		
		if (drX != 0)
			dx = (scrWidth / 2) / drX;
		if (dbY != 0)
			dy = (scrHeight / 2) / dbY;

		nx = scrWidth / 2 - abs(cX - eyeCenter.x) * dx;
		ny = scrHeight / 2 + abs(cY - eyeCenter.y) * dy;
	}	 
	circle(canvas, Point(nx, ny), 5, Scalar(255, 255, 255), 2);
	
	return true;
}

bool Calibrator::processMode6Regions(ApplicationState &appState)
{
	Point eyeCenter = appState.rEyeRelativeCenterPoint;

	canvas = Mat::zeros(Size(scrWidth, scrHeight), CV_8U);
	
	int d = 9;

	if (eyeCenter.x < mCenterX - d) {
		if (eyeCenter.y > mCenterY) {
			rectangle(canvas, Rect((scrWidth / 3) * 2, scrHeight / 2, scrWidth / 3, scrHeight / 2), Scalar(255, 255, 255), 2);
		} else {
			rectangle(canvas, Rect((scrWidth / 3) * 2, 0, scrWidth / 3, scrHeight/2), Scalar(255, 255, 255), 2);
		}
	}
	if (eyeCenter.x > mCenterX + d)
	{
		if (eyeCenter.y > mCenterY) {
			rectangle(canvas, Rect(0, scrHeight / 2, scrWidth / 3, scrHeight / 2), Scalar(255, 255, 255), 2);
		}
		else {
			rectangle(canvas, Rect(0, 0, scrWidth / 3, scrHeight/2), Scalar(255, 255, 255), 2);
		}
	}
	else if(eyeCenter.x > (mCenterX - d) && eyeCenter.x < mCenterX + d) {
		if (eyeCenter.y > mCenterY) {
			rectangle(canvas, Rect(scrWidth / 3, scrHeight / 2, scrWidth / 3, scrHeight / 2), Scalar(255, 255, 255), 2);
		}
		else {
			rectangle(canvas, Rect(scrWidth / 3, 0, scrWidth / 3, scrHeight/2), Scalar(255, 255, 255), 2);
		}
	}
	

	return true;
}

bool Calibrator::collectCalibrationData(ApplicationState &appState)
{
	Point eyeCenter;
	if (appState.usedEye == EYE_LEFT) {
		eyeCenter = appState.rEyeRelativeCenterPoint;
	}
	else {
		eyeCenter = appState.lEyeRelativeCenterPoint;
	}

	canvas = Mat::zeros(Size(1920, 1000), CV_8U);

	int headDx = abs(appState.headAproxCenterPoint.x - appState.headCenterPoint.x);
	int headDy = abs(appState.headAproxCenterPoint.y - appState.headCenterPoint.y);
	if (headDx < 3 && headDy < 3) {
		this->headCentersVector.push_back(appState.headAproxCenterPoint);
	}

	/*GET CENTER POINTS*/
	while (centerX.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 2, canvas.rows / 2), 50, Scalar(255, 255, 255), 3);

		centerX.push_back(eyeCenter.x);
		centerY.push_back(eyeCenter.y);

		imshow("Canvas", canvas);
		return false;
	}

	//Showing four points in the conrens obtains more accuracy calibration data!
	while (leftX.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 - 50, canvas.rows / 4 - 50), 20, Scalar(255, 255, 255), 2);
		
		leftX.push_back(eyeCenter.x);
		topY.push_back(eyeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (leftX.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 4 - 50, canvas.rows / 4 * 3 + 50), 20, Scalar(255, 255, 255), 2);
		
		leftX.push_back(eyeCenter.x);
		bottomY.push_back(eyeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (rightX.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 * 3 + 50, canvas.rows / 4 - 50), 20, Scalar(255, 255, 255), 2);
		
		rightX.push_back(eyeCenter.x);
		topY.push_back(eyeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (rightX.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 4 * 3 + 50, canvas.rows / 4 * 3 + 50), 20, Scalar(255, 255, 255), 2);
		
		rightX.push_back(eyeCenter.x);
		bottomY.push_back(eyeCenter.y);
	
		imshow("Canvas", canvas);
		return false;
	}

	return true;
}

void Calibrator::computeCallibrationData(ApplicationState &appState)
{
	int zLx = 0;
	int zRx = 0;
	int zCx = 0;
	int zCy = 0;
	int zTy = 0;
	int zBy = 0;
	
	int c = 0;

	std::sort(leftX.begin(), leftX.end());
	std::sort(rightX.begin(), rightX.end());
	std::sort(centerX.begin(), centerX.end());
	std::sort(centerY.begin(), centerY.end());
	std::sort(topY.begin(), topY.end());
	std::sort(bottomY.begin(), bottomY.end());

	for (int i = 0; i < leftX.size(); i++) {
		if (zLx != leftX[i]) {
			zLx = leftX[i];
			c = std::count(leftX.begin(), leftX.end(), zLx);
			sortedLx.push_back(Point(zLx,c));
		}
		if (zRx != rightX[i]) {
			zRx = rightX[i];
			c = std::count(rightX.begin(), rightX.end(), zRx);
			sortedRx.push_back(Point(zRx,c));
		}
		if (zTy != topY[i]) {
			zTy = topY[i];
			c = std::count(topY.begin(), topY.end(), zTy);
			sortedTy.push_back(Point(zTy, c));
		}
		if (zBy != bottomY[i]) {
			zBy = bottomY[i];
			c = std::count(bottomY.begin(), bottomY.end(), zBy);
			sortedBy.push_back(Point(zBy, c));
		}
	}

	for (int i = 0; i < centerX.size(); i++) {
		if (zCx != centerX[i]) {
			zCx = centerX[i];
			c = std::count(centerX.begin(), centerX.end(), zCx);
			sortedCx.push_back(Point(zCx, c));
		}
		if (zCy != centerY[i]) {
			zCy = centerY[i];
			c = std::count(centerY.begin(), centerY.end(), zCy);
			sortedCy.push_back(Point(zCy, c));
		}
	}
	 
	std::sort(sortedCx.begin(), sortedCx.end(), _internal_sort_points);
	std::sort(sortedLx.begin(), sortedLx.end(), _internal_sort_points);
	std::sort(sortedRx.begin(), sortedRx.end(), _internal_sort_points);
	std::sort(sortedTy.begin(), sortedTy.end(), _internal_sort_points);
	std::sort(sortedBy.begin(), sortedBy.end(), _internal_sort_points);
	std::sort(sortedCy.begin(), sortedCy.end(), _internal_sort_points);

	int valL = 0, sumL = 0, valR = 0, sumR = 0, valC = 0, sumC = 0, valTy = 0, sumTy = 0, valCy = 0, sumCy = 0;

	for (int i = 0; i < 4; i++) {
		valL += sortedLx[i].x * sortedLx[i].y;
		sumL += sortedLx[i].y;
	}

	for (int i = 0; i < 4; i++) {
		valR += sortedRx[i].x * sortedRx[i].y;
		sumR += sortedRx[i].y;
	}

	for (int i = 0; i < 4; i++) {
		valC += sortedCx[i].x * sortedCx[i].y;
		sumC += sortedCx[i].y;
	}

	for (int i = 0; i < 4; i++) {
		valTy += sortedTy[i].x * sortedTy[i].y;
		sumTy += sortedTy[i].y;
	}

	for (int i = 0; i < 4; i++) {
		valCy += sortedCy[i].x * sortedCy[i].y;
		sumCy += sortedCy[i].y;
	}

	this->mLeftX = valL / sumL;
	this->mRightX = valR / sumR;
	this->mCenterX = valC / sumC;
	this->mCenterY = valCy / sumCy;

	int mx = 0, my = 0;
	int cx = 0, cy = 0;

	for (int i = 0; i < this->headCentersVector.size(); i++) {
		mx += this->headCentersVector[i].x;
		my += this->headCentersVector[i].y;
	}

	if (this->headCentersVector.size() > 0)
	{
		mx = mx / this->headCentersVector.size();
		my = my / this->headCentersVector.size();
	}

	appState.eyeMeanCenterPoint = Point(mCenterX, mCenterY);

	//Compute initial mx and dx value.

	if (appState.headAngleRef == HEAD_NOSE){
		appState.initHeadOX = appState.noseAproxCenterPoint.y - my;
		appState.initHeadOY = appState.noseAproxCenterPoint.x - mx;
	}
	else {
		appState.initHeadOX = appState.mouthAproxCenterPoint.y = my;
		appState.initHeadOY = appState.mouthAproxCenterPoint.x - mx;
	}

	appState.leftFactor = abs(appState.eyeMeanCenterPoint.x - this->mLeftX) / 960.0;
	appState.rightFactor = abs(appState.eyeAproxCenterPoint.x - this->mRightX) / 960.0;

	std::cout <<"==========================COMPUTED DATA=============================" << std::endl;
	std::cout << "EyemeanCenterPoint" << appState.eyeMeanCenterPoint << std::endl;
	std::cout << "InitHeadOX" << appState.initHeadOX << std::endl;
	std::cout << "InitHeadOY" << appState.initHeadOY << std::endl;
	std::cout << "Mleft" << appState.leftFactor<< std::endl;
	std::cout << "MRight" << appState.rightFactor << std::endl;
	std::cout << "===================================================================" << std::endl;

	appState.isCalibrated = true;
}

bool Calibrator::getCalibrationStatus(void) 
{
	return isCalibrated;
}