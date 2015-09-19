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
	namedWindow("Canvas", CV_WINDOW_FULLSCREEN);
	moveWindow("Canvas", 0, 0);
	isCalibrationDataCollected = false;
	isCalibrated = false;

	cX = 0; cY = 0; lX = 0; rX = 0; tY = 0; bY = 0;
	mHeadX = 0; mHeadY = 0; mNoseX = 0; mNoseY = 0; mMouthX = 0; mMouthY = 0;
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
	if (isCalibrationDataCollected == false) {
		if (!collectCalibrationData(appState))
			return true;
		isCalibrationDataCollected = true;
	}

	if (isCalibrated == false) {
		computeCallibrationData(appState);
		isCalibrated = true;
		appState.isCalibrated = true;
	}  else {
		destroyWindow("Canvas");
		return true;
	}

	imshow("Canvas", canvas);
	return true;
}

void Calibrator::moduleDeinit(void)
{

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
	Point eyeCenter = appState.eyeCenter;

	canvas = Mat::zeros(Size(1920, 1000), CV_8U);

	this->headCentersVector.push_back(appState.headAproxCenterPoint);
	this->noseCentersVector.push_back(appState.noseCenterPoint);
	this->mouthCentersVector.push_back(appState.mouthCenterPoint);

	while (centerX.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 2, canvas.rows / 2), 50, Scalar(255, 255, 255), 3);

		centerX.push_back(eyeCenter.x);
		centerY.push_back(eyeCenter.y);

		imshow("Canvas", canvas);
		return false;
	}

	//Showing four points in the conrens obtains more accuracy calibration data!
	while (leftX.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 - 250, canvas.rows / 4 - 150), 50, Scalar(255, 255, 255), 3);
		
		leftX.push_back(eyeCenter.x);
		topY.push_back(eyeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (leftX.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 4 - 250, canvas.rows / 4 * 3 + 150), 50, Scalar(255, 255, 255), 3);
		
		leftX.push_back(eyeCenter.x);
		bottomY.push_back(eyeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (rightX.size() < dCounter) {
		circle(canvas, Point(canvas.cols / 4 * 3 + 250, canvas.rows / 4 - 150), 50, Scalar(255, 255, 255), 3);
		
		rightX.push_back(eyeCenter.x);
		topY.push_back(eyeCenter.y);
		
		imshow("Canvas", canvas);
		return false;
	}

	while (rightX.size() < dCounter * 2) {
		circle(canvas, Point(canvas.cols / 4 * 3 + 250, canvas.rows / 4 * 3 + 150), 50, Scalar(255, 255, 255), 3);
		
		rightX.push_back(eyeCenter.x);
		bottomY.push_back(eyeCenter.y);
	
		imshow("Canvas", canvas);
		return false;
	}

	return true;
}

void Calibrator::computeCallibrationData(ApplicationState &appState)
{
	int zLx = -2;
	int zRx = -2;
	int zCx = -2;
	int zCy = -2;
	int zTy = -2;
	int zBy = -2;
	
	int c = 0;

	std::sort(leftX.begin(), leftX.end());
	std::sort(rightX.begin(), rightX.end());
	std::sort(centerX.begin(), centerX.end());
	std::sort(centerY.begin(), centerY.end());
	std::sort(topY.begin(), topY.end());
	std::sort(bottomY.begin(), bottomY.end());


	for (int i = 0; i < leftX.size(); i++) {
		printf("I[%d], LeftXSize[%d], zLx[%d]\n", i, leftX.size(), zLx);
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
	for (int i = 0; i < 2; i++) {
		valL += sortedLx[i].x * sortedLx[i].y;
		sumL += sortedLx[i].y;
	}

	for (int i = 0; i < 2; i++) {
		valR += sortedRx[i].x * sortedRx[i].y;
		sumR += sortedRx[i].y;
	}

	for (int i = 0; i < 2; i++) {
		valC += sortedCx[i].x * sortedCx[i].y;
		sumC += sortedCx[i].y;
	}

	for (int i = 0; i < 2; i++) {
		valTy += sortedTy[i].x * sortedTy[i].y;
		sumTy += sortedTy[i].y;
	}

	for (int i = 0; i < 2; i++) {
		valCy += sortedCy[i].x * sortedCy[i].y;
		sumCy += sortedCy[i].y;
	}

	this->mLeftX = valL / sumL;
	this->mRightX = valR / sumR;
	this->mCenterX = valC / sumC;
	this->mCenterY = valCy / sumCy;

	appState.eyeMeanCenterPoint = Point(mCenterX, mCenterY);
	appState.leftFactor = abs(appState.eyeMeanCenterPoint.x - this->mLeftX) / 960.0;
	appState.rightFactor = abs(appState.eyeMeanCenterPoint.x - this->mRightX) / 960.0;
	appState.maxLeft = this->mLeftX;
	appState.maxRight = this->mRightX;

	int cnt = 0;
	for (int i = 10; i < this->headCentersVector.size(); i++) {
		cnt++;
		mHeadX += this->headCentersVector[i].x;
		mHeadY += this->headCentersVector[i].y;
		mNoseX += this->noseCentersVector[i].x;
		mNoseY += this->noseCentersVector[i].y;
		mMouthX += this->mouthCentersVector[i].x;
		mMouthY += this->mouthCentersVector[i].y;

		//std::cout << "HEAD_CENTER: " << this->headCentersVector[i] << std::endl;
		//std::cout << "NOSE_CENTER: " << this->mouthCentersVector[i] << std::endl;
		//std::cout << "MOUTH_CENTER: " << this->noseCentersVector[i] << std::endl;
	}

	//std::cout << "counter : " << cnt << std::endl;
		 
	if (this->headCentersVector.size() > 0)
	{
		mHeadX = mHeadX / cnt;
		mHeadY = mHeadY / cnt;

		mNoseX = mNoseX / cnt;
		mNoseY = mNoseY / cnt;
		
		mMouthX = mMouthX / cnt;
		mMouthY = mMouthY / cnt;

		//std::cout << "MHEAD: " << mHeadX << " " << mHeadY << " MNOSE" << mNoseX << " " << mNoseY << std::endl;
	}

	//Compute initial mx and dx value.

	if (appState.headAngleRef == HEAD_NOSE){
		appState.initHeadOX = mHeadX - mNoseX;
		appState.initHeadOY = mHeadY - mNoseY;
	}
	else {
		appState.initHeadOX = mHeadX - mMouthX;
		appState.initHeadOY = mHeadY - mMouthY;
	}

	std::cout <<"==========================COMPUTED DATA=============================" << std::endl;
	std::cout << "EyemeanCenterPoint" << appState.eyeMeanCenterPoint << std::endl;
	std::cout << "InitHeadOX" << appState.initHeadOX << std::endl;
	std::cout << "InitHeadOY" << appState.initHeadOY << std::endl;
	std::cout << "Left Factor" << appState.leftFactor<< std::endl;
	std::cout << "Right Factor" << appState.rightFactor << std::endl;
	std::cout << "mRight" << this->mRightX << std::endl;
	std::cout << "mLeft" << this->mLeftX << std::endl;
	
	std::cout << "Left Top Point X" << mLeftX << " Y " << mTopY << std::endl;
	std::cout << "Left Bottom Point X " << mLeftX << " Y " << mBottomY << std::endl;
	std::cout << "Right Top Point X " << mRightX << " Y " << mTopY << std::endl;
	std::cout << "Right Bottom Point X" << mRightX << " Y " << mBottomY << std::endl;

 	std::cout << "===================================================================" << std::endl;

	appState.isCalibrated = true;
}

bool Calibrator::getCalibrationStatus(void) 
{
	return isCalibrated;
}