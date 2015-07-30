#include "CursorController.h"


bool _internal_sort_points(Point p1, Point p2)
{
	if (p1.y > p2.y)
		return true;
	else
		return false;
}

CursorController::CursorController() 
{
	namedWindow("Canvas", 0);
	isCalibrationDataCollected = false;
	isCalibrated = false;

	cX = 0; cY = 0; lX = 0; rX = 0; tY = 0; bY = 0;
	canvas = Mat::zeros(Size(scrWidth, scrHeight), CV_8U);
}

CursorController::~CursorController()
{

}

void CursorController::moduleInit(ApplicationState &appState)
{
	appState.apMode = APPLICATION_MODE_6_REGIONS;
}

bool CursorController::moduleProcess(ApplicationState &appState)
{
	Point eyeCenter = appState.lEyeCenter;

	if (isCalibrationDataCollected == false) {
		
		if (!collectCalibrationData(appState))
			return true;

		isCalibrationDataCollected = true;
	}

	if (isCalibrated == false) {
		computeCallibrationData(appState);
		isCalibrated = true;
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

void CursorController::moduleDeinit(void)
{

}

bool CursorController::processModeGazeTracker(ApplicationState &appState)
{
	Point eyeCenter = appState.lEyeRelativeCenter;
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

bool CursorController::processMode6Regions(ApplicationState &appState)
{
	Point eyeCenter = appState.rEyeRelativeCenter;

	canvas = Mat::zeros(Size(scrWidth, scrHeight), CV_8U);
	
	std::cout << "x: " << eyeCenter.x << "y: " << eyeCenter.y << std::endl;

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

bool CursorController::collectCalibrationData(ApplicationState &appState)
{
	Point eyeCenter = appState.rEyeCenter;
	canvas = Mat::zeros(Size(scrWidth, scrHeight), CV_8U);

	int headDx = abs(appState.headAproxCenter.x - appState.headCenter.x);
	int headDy = abs(appState.headAproxCenter.y - appState.headCenter.y);

	if (headDx < 3 && headDy < 3) {
		this->headCentersVector.push_back(appState.headAproxCenter);
	}

	while (cPoints.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 2, canvas.rows / 2), 20, Scalar(255, 255, 255), 2);
		cPoints.push_back(eyeCenter);
		centerX.push_back(appState.rEyeRelativeCenter.x);
		centerY.push_back(appState.rEyeRelativeCenter.y);

		imshow("Canvas", canvas);
		return false;
	}

	//Showing four points in the conrens obtains more accuracy calibration data!
	while (ltPoints.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 - 50, canvas.rows / 4 - 50), 20, Scalar(255, 255, 255), 2);
		ltPoints.push_back(eyeCenter);
		
		leftX.push_back(appState.rEyeRelativeCenter.x);
		topY.push_back(appState.rEyeRelativeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (lbPoints.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 - 50, canvas.rows / 4 * 3 + 50), 20, Scalar(255, 255, 255), 2);
		lbPoints.push_back(eyeCenter);
		
		leftX.push_back(appState.rEyeRelativeCenter.x);
		bottomY.push_back(appState.rEyeRelativeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (rtPoints.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 * 3 + 50, canvas.rows / 4 - 50), 20, Scalar(255, 255, 255), 2);
		rtPoints.push_back(eyeCenter);
		
		rightX.push_back(appState.rEyeRelativeCenter.x);
		topY.push_back(appState.rEyeRelativeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (rbPoints.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 * 3 + 50, canvas.rows / 4 * 3 + 50), 20, Scalar(255, 255, 255), 2);
		rbPoints.push_back(eyeCenter);
		
		rightX.push_back(appState.rEyeRelativeCenter.x);
		bottomY.push_back(appState.rEyeRelativeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	return true;
}

void CursorController::computeCallibrationData(ApplicationState &appState)
{
	for (int i = 0; i < dCounter; i++) {
		this->cX += cPoints[i].x;
		this->cY += cPoints[i].y;

		//std::cout << cX << " " << cY << std::endl;

		this->lX += ltPoints[i].x;
		this->lX += lbPoints[i].x;

		this->rX += rtPoints[i].x;
		this->rX += rbPoints[i].x;

		this->tY += rtPoints[i].y;
		this->tY += ltPoints[i].y;

		this->bY += rbPoints[i].y;
		this->bY += lbPoints[i].y;
	}

	this->cX = this->cX / dCounter;
	this->cY = this->cY / dCounter;

	this->rX = this->rX / (dCounter * 2);
	this->lX = this->lX / (dCounter * 2);

	this->tY = this->tY / (dCounter * 2);
	this->bY = this->bY / (dCounter * 2);

	drX = abs(cX - rX);
	dlX = abs(cX - lX);

	dtY = abs(cY - tY);
	dbY = abs(cY - bY);

	/*Helper values for sort and count vectors*/
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

	for (int i = 0; i < this->headCentersVector.size(); i++) {
		mx += this->headCentersVector[i].x;
		my += this->headCentersVector[i].y;
	}

	if (this->headCentersVector.size() > 0)
	{
		mx = mx / this->headCentersVector.size();
		my = my / this->headCentersVector.size();
	}

	appState.headForScreenCenter = Point(mx, my);
	appState.oxForScreenCenter = appState.headAproxCenter.y - appState.mouthAproxCenter.y;
	appState.oyForScreenCenter = appState.headAproxCenter.x - appState.mouthAproxCenter.x;
}


bool CursorController::getCalibrationStatus(void) 
{
	return isCalibrated;
}