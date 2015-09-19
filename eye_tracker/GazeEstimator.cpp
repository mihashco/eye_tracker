#include "GazeEstimator.h"

#include <sstream>
#include <iomanip>
#include <iostream>

Point getRegion(int scrWidth, int scrHeight, int verticalRegions, int horizontalRegions, Point out)
{
	int regionWidth = scrWidth / horizontalRegions;
	int regionHeight = scrHeight / verticalRegions;

	Point ret;
	ret.x = out.x / regionWidth;
	ret.y = out.y / regionHeight;

	return ret;
}


GazeEstimator::GazeEstimator()
{
	std::cout << "Gaze constructor" << std::endl;

	Point center = Point(1920 / 2, 1080 / 2);
	namedWindow("GAZE", CV_WINDOW_AUTOSIZE);
	moveWindow("GAZE", 0, 0);
	resizeWindow("GAZE", 0, 0);

	/*outKF.setProcessNoiseCov(5.0);
	outKF.setMeasurementNoiseCov(5.0);
	outKF.setErrorCovPost(5.0);*/
	
	testResults.open("testResults.txt");
	currentTC = 0;
	currentRegion = 0;

	this->winEyeVisible = false;
	this->winHeadVisible = false;

	heatMap = Mat::zeros(Size(1920, 1080), CV_8UC1);
	heatMapTestImage = imread("C:\\Users\\mszcz_000\\Desktop\\fhd\\test1.jpg");

	regionsOutData = new int *[3];
	for (int i = 0; i < 3; i++)
	{
		regionsOutData[i] = new int[2];
	}
}

GazeEstimator::~GazeEstimator()
{
	if(testResults.good() == true) {
		testResults.close();
	}
}

void GazeEstimator::moduleInit(ApplicationState &appState)
{
	this->heatMap = Mat::zeros(Size(1920, 1080), CV_8UC3);
}

bool GazeEstimator::moduleProcessGazeDebug(ApplicationState &appState, Point out)
{
	Mat outHeadMat = Mat::zeros(Size(200, 200), CV_8UC3);
	Mat outEyeMat = Mat::zeros(Size(200, 200), CV_8UC3);
	Mat gazeCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
	
	if (!this->winEyeVisible) {
		namedWindow("WinEye", CV_WINDOW_AUTOSIZE);
		moveWindow("WinEye", 0, 0);
		this->winEyeVisible = true;
	}

	if (!this->winHeadVisible) {
		namedWindow("WinHead", CV_WINDOW_AUTOSIZE);
		moveWindow("WinHead", 0, 200);
		this->winHeadVisible = true;
	}

	Point center;
	Point headLineEnd;
	Point eyeLineEnd;

	center.x = 100; center.y = 100;
	headLineEnd.x = 100 + appState.deltaHeadOX;
	headLineEnd.y = 100 + appState.deltaHeadOY;
	eyeLineEnd.x = 100 + appState.eyeDeltaX;
	eyeLineEnd.y = 100 + appState.eyeDeltaY;

	line(outHeadMat, center, headLineEnd, Scalar(0, 0, 255), 3);
	line(outEyeMat, center, eyeLineEnd, Scalar(0, 0, 255), 3);

	std::stringstream gazeX;
	std::stringstream gazeY;

	gazeX << "Eye X " << appState.eyeCenter.x;
	gazeY << "Eye Y " << appState.eyeCenter.y;

	putText(outEyeMat, gazeX.str(), Point(0, 20), 2, 1, Scalar(255, 0, 0), 2);
	putText(outEyeMat, gazeY.str(), Point(0, 50), 2, 1, Scalar(255, 0, 0), 2);

	circle(gazeCanvas, out, 70, Scalar(0, 0, 255), 3);
	imshow("WinEye", outEyeMat);
	imshow("WinHead", outHeadMat);
	imshow("GAZE", gazeCanvas);
	return true;
}

bool GazeEstimator::moduleProcessGazeHeatMap(ApplicationState &appState, Point out)
{
	static int tick = 0;
	static int init = 0;

	std::cout << "ModuleProcessGazeHeatMap [" << tick << "]" << std::endl;

	if (!init) {
		namedWindow("HeatMap", 1);
		moveWindow("HeatMap", 0, 0);
	}

	init = 1;

	if (tick >= appState.measureTime) {
		std::cout << "Heat Map computed " << std::endl;
		normalize(heatMap, heatMap, 0, 255, NORM_MINMAX, CV_8UC1);
		blur(heatMap, heatMap, Size(50, 50));
		applyColorMap(heatMap, heatMap, COLORMAP_HOT);
		heatMapTestImage = heatMapTestImage + heatMap;
		imshow("HeatMap", heatMapTestImage);
		std::cout << "HeatMap Finshed!" << std::endl;
		return false;
	}

	if (out.x >= 0 && out.x < heatMap.size().width && out.y >= 0 && out.y < heatMap.size().height) {
		uchar v = heatMap.at<uchar>(out);
		if (v < 245) v += 10;
		circle(heatMap, out, 40, Scalar(v), 80);
	}

	imshow("HeatMap", heatMap);
	
	tick++;
	imshow("HeatMap", heatMapTestImage);
	return true;
}

bool GazeEstimator::moduleProcessGazeTest(ApplicationState &appState, Point out)
{
	Mat testCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
	int testCasesPerRegion = 100;
	int regions = 6;

	int rectWidth = 1920 / 3;
	int rectHeight = 1080 / 2;

	Point ref;

	switch (currentRegion) {
		case 0:
			circle(testCanvas, Point(640 / 2, 540 / 2), 100, Scalar(0, 0, 255), 5);
			ref = Point(640 / 2, 540 / 2);

			if (currentTC == 0) {
				testResults << "Test Point #0" << std::endl;
			}

			currentTC++;
			if (currentTC == testCasesPerRegion) {
				currentTC = 0;
				currentRegion++;
			}
			break;
		case 1:
			circle(testCanvas, Point(640 + 640 / 2, 540 / 2), 100, Scalar(0, 0, 255), 5);
			ref = Point(640 + 640 / 2, 540 / 2);

			if (currentTC == 0) {
				testResults << "Test Point #1" << std::endl;
			}
			currentTC++;

			if (currentTC == testCasesPerRegion) {
				currentTC = 0;
				currentRegion++;
			}

			break;
		case 2:
			circle(testCanvas, Point(640 * 2 + 640 / 2, 540 / 2), 100, Scalar(0, 0, 255), 5);
			ref = Point(640 * 2 + 640 / 2, 540 / 2);

			if (currentTC == 0) {
				testResults << "Test Point #2" << std::endl;
			}

			currentTC++;
			if (currentTC == testCasesPerRegion) {
				currentTC = 0;
				currentRegion++;
			}

			break;
		case 3:
			circle(testCanvas, Point(640 / 2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
			ref = Point(640 / 2, 540 + 540 / 2);

			if (currentTC == 0) {
				testResults << "Test Point #3" << std::endl;
			}

			currentTC++;

			if (currentTC == testCasesPerRegion) {
				currentTC = 0;
				currentRegion++;
			}
			break;
		case 4:
			circle(testCanvas, Point(640 + 640 / 2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
			ref = Point(640 + 640 / 2, 540 + 540 / 2);

			if (currentTC == 0) {
				testResults << "Test Point #4" << std::endl;
			}

			currentTC++;

			if (currentTC == testCasesPerRegion) {
				currentTC = 0;
				currentRegion++;
			}
			break;
		case 5:
			circle(testCanvas, Point(640 * 2 + 640 / 2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
			ref = Point(640 * 2 + 640 / 2, 540 + 540 / 2);

			if (currentTC == 0) {
				testResults << "Test Point #5" << std::endl;
			}

			currentTC++;

			if (currentTC == testCasesPerRegion) {
				currentTC = 0;
				currentRegion++;
			}
			break;
		default:
			break;
	}

	circle(testCanvas, out, 200, Scalar(255, 0, 0), 3);

	if (appState.noFace) {
		putText(testCanvas, "No face detected", Point(10, 10), 1, 1, Scalar(0, 0, 255), 4);
	}
	imshow("GAZE", testCanvas);

	double absoluteErrorX = abs(ref.x - out.x);
	double absoluteErrorY = abs(ref.y - out.y);

	double relativeErrorX = 0.0;
	double relativeErorrY = 0.0;

	if (out.x > 0 && out.y > 0)
	{
		relativeErrorX = (double)abs(ref.x - out.x) / out.x;
		relativeErorrY = (double)abs(ref.y - out.y) / out.y;
	}

	testResults << "[" << currentTC << "]\t" << out.x << "\t" << out.y << "\t" << absoluteErrorX << "\t" << absoluteErrorY << "\t" << relativeErrorX << "\t" << relativeErorrY << std::endl;

	return true;
}

bool GazeEstimator::moduleProcessGazeSixRegions(ApplicationState &appState, Point out)
{
	Mat gazeCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
	Mat regionsTestImg = this->heatMapTestImage.clone();

	//TODO: this should be changed
	int screenWidth = 1920;
	int screenHeight = 1080;
	int vReg = 2;
	int hReg = 3;

	int regWidth = screenWidth / hReg;
	int regHeight = screenHeight / vReg;
	static int testCount = 0;

	Point current_region = getRegion(1980, 1080, 2, 3, out);
	Point cr_top_point(current_region.x * regWidth, current_region.y * regHeight);
	Point cr_bottom_point(cr_top_point.x + regWidth, cr_top_point.y + regHeight);

	regionsOutData[current_region.x][current_region.y]++;
	testCount++;
	
	double status = (double)(testCount / 300.0) * 1920;
	rectangle(regionsTestImg, Point(0, 0), Point(status, 5), Scalar(0, 255, 0), CV_FILLED);

	if (testCount == 300) {
		for (int j = 0; j < vReg; j++) {
			for (int i = 0; i < hReg; i++) {
				double percentageFocus = (((double) regionsOutData[i][j] / testCount)) * 100.0;

				Point tLeft(i * regWidth, j * regHeight);
				Point bRight(i * regWidth + regWidth, j * regHeight + regHeight);
				Rect area(tLeft, bRight);

				std::stringstream ss;
				ss << "[" << std::setprecision(3) << percentageFocus << "%]";

				putText(regionsTestImg, ss.str(), Point(area.x + area.width / 2, area.y + area.height / 2), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 3);
				imshow("GAZE", regionsTestImg);
			}
		}

		return true;
	}

	rectangle(regionsTestImg, cr_top_point, cr_bottom_point, Scalar(0, 0, 255), 3);

	imshow("GAZE", regionsTestImg);
	return false;
}

bool GazeEstimator::moduleProcess(ApplicationState &appState)
{
	static bool stop = false;

	if (!appState.isCalibrated)
		return true;

	static int init = 0;
	if (!init) {
		resizeWindow("GAZE", 1920, 1080);
	}

	double usedFactor;
	if (appState.eyeDeltaX > 0)
		usedFactor = appState.rightFactor * 2;
	else
		usedFactor = appState.leftFactor * 2;

	Point out;
	out.x = 960 + appState.headOX * 30 + appState.eyeDeltaX / usedFactor;
	out.y = 540 + appState.deltaHeadOY * 20 + appState.eyeDeltaY * 30;
	out = outKF.getPoint(out);

	if (out.x >= 1920)
		out.x = 1920;
	if (out.y >= 1080)
		out.y = 1080;
	if (out.x <= 0)
		out.x = 0;
	if (out.y <= 0)
		out.y = 0;
	
	if (appState.gzMode == GAZE_DEBUG) {
		if (!moduleProcessGazeDebug(appState, out))
			return false;
	} else if (appState.gzMode == GAZE_TEST) {
		if (!moduleProcessGazeTest(appState, out))
			return false;
	} else if (appState.gzMode == GAZE_HEAT_MAP && stop != true) {
		stop = moduleProcessGazeHeatMap(appState, out);
	} else if (appState.gzMode == GAZE_SIX_REGIONS && stop != true) {
		stop = moduleProcessGazeSixRegions(appState, out);
	} else if (appState.gzMode == GAZE_CURSOR_CONTROLLER) {

	}

	return true;
}

void GazeEstimator::moduleDeinit(void)
{

}
