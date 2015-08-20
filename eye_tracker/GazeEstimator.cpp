#include "GazeEstimator.h"


GazeEstimator::GazeEstimator()
{
	std::cout << "Gaze constructor" << std::endl;

	this->rotation = Mat(2, 3, CV_8U);
	Point center = Point(1920 / 2, 1080 / 2);
	double angle = 180.0;
	double scale = 1.0;
	this->rotation = getRotationMatrix2D(center, angle, scale);
	
	//measureTime = 0;

	//visited = Mat::zeros(Size(1920, 1080), CV_8UC1);
	//checkedImage = imread("C:\\Users\\mszcz_000\\Desktop\\fhd\\test1.jpg");
	namedWindow("GAZE", 0);
	moveWindow("GAZE", 0, 0);

	outKF.setProcessNoiseCov(100.0);
	outKF.setMeasurementNoiseCov(120.0);
	outKF.setErrorCovPost(100.0);
	
	testResults.open("testResults.txt");
	currentTC = 0;
	currentRegion = 0;
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

bool GazeEstimator::moduleProcess(ApplicationState &appState)
{
	if (!appState.isCalibrated)
		return true;

	gazeCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
	double usedFactor;

	if (appState.eyeDeltaX > 0) {
		usedFactor = appState.rightFactor * 3.0;
	}
	else {
		usedFactor = appState.leftFactor * 1.2;
	}

	Point out(appState.headCenterPoint.x - appState.eyeDeltaX / usedFactor, appState.headCenterPoint.y - appState.eyeDeltaY * 100);

	out = outKF.getPoint(out);
	circle(gazeCanvas, out, 200, Scalar(255, 0, 0), 3);

	if (appState.gzMode == GAZE_DEBUG) {
		//warpAffine(gazeCanvas, gazeCanvas, this->rotation, gazeCanvas.size());
		//flip(gazeCanvas, gazeCanvas, 0);
		//std::cout << out << std::endl;
		imshow("GAZE_DEBUG", gazeCanvas);
	} 
	else if (appState.gzMode == GAZE_TEST) {
		Mat testCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
		int testCasesPerRegion = 100; //TODO: move this to the AppState
		int regions = 6; //TODO: move this to the AppState
		
		int rectWidth = 1920 / 3;
		int rectHeight = 1080 / 2;
		
		Point ref;
		
		switch (currentRegion) {
			case 0:
				circle(testCanvas, Point(640/2, 540 / 2), 100, Scalar(0, 0, 255), 5);
				ref = Point(640 / 2, 540 / 2);

				if(currentTC == 0) {
					testResults << "Test Point #0" << std::endl;	
				}
		
				currentTC++;
				if(currentTC == testCasesPerRegion) {
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
				
				if(currentTC == testCasesPerRegion) {
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
				if(currentTC == testCasesPerRegion) {
					currentTC = 0;
					currentRegion++;
				}
				
				break;
			case 3:
				circle(testCanvas, Point(640/2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
				ref = Point(640 / 2, 540 + 540 / 2);

				if (currentTC == 0) {
					testResults << "Test Point #3" << std::endl;
				}

				currentTC++;
				
				if(currentTC == testCasesPerRegion) {
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
				
				if(currentTC == testCasesPerRegion) {
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
				
				if(currentTC == testCasesPerRegion) {
					currentTC = 0;
					currentRegion++;
				}
				break;
			default:
				break;
		}

		circle(testCanvas, out, 200, Scalar(255, 0, 0), 3);
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

		testResults << "[" << currentTC << "]\t" << absoluteErrorX << "\t" << absoluteErrorY << "\t" << relativeErrorX << "\t" << relativeErorrY << std::endl;
	}

	return true;
}

void GazeEstimator::moduleDeinit(void)
{

}
