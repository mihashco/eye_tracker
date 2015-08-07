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
	namedWindow("GAZE_DEBUG", 0);
	moveWindow("GAZE_DEBUG", 0, 0);

	outKF.setProcessNoiseCov(100.0);
	outKF.setMeasurementNoiseCov(120.0);
	outKF.setErrorCovPost(100.0);
	
	testResults.open("testResults.txt");
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
	gazeCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
	double usedFactor;

	if (appState.eyeDeltaX > 0) {
		usedFactor = appState.rightFactor * 3.0;
	}
	else {
		usedFactor = appState.leftFactor * 1.2;
	}

	Point out(appState.headCenterPoint.x - appState.eyeDeltaX / usedFactor, appState.headCenterPoint.y - appState.eyeDeltaY);

	out = outKF.getPoint(out);
	circle(gazeCanvas, out, 200, Scalar(255, 0, 0), 3);

	if (appState.gzMode == GAZE_DEBUG) {
		warpAffine(gazeCanvas, gazeCanvas, this->rotation, gazeCanvas.size());
		flip(gazeCanvas, gazeCanvas, 0);
		std::cout << out << std::endl;
		imshow("GAZE_DEBUG", gazeCanvas);
	} 
	else if (appSAtet.gzMode == GAZE_TEST) {
		Mat testCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
		int testCasesPerRegion = 100; //TODO: move this to the AppState
		int regions = 6; //TODO: move this to the AppState
		int currentTC = 0;
		int currentRegion = 0;
		
		int rectWidth = 1920 / 3;
		int rectHeight = 1080 / 2;
		
		static vector<vector<Point> > testData;
		
		switch (currentRegion) {
			case 0:
				circle(testCanvas, Point(640/2, 540 / 2), 100, Scalar(0, 0, 255), 5);
				static vector<Point> tc1;
				tc1.push_back(out);
				
				Point ref(640 / 2, 540 / 2);
				if(currentTC == 0) {
					testResults << "Test Point #0" << std::endl;	
				}
				
				double absoluteErrorX = abs(ref.x - out.x);
				double absoluteErrorY = abs(ref.y - out.y);
				
				if(out.x >= 0 && out.y >= 0)
					relativeErrorX = abs(ref.x - out.x) / out.x
					relativeErrorY = abs(ref.y - out.y) / out.y
				
				testResults << "[" << currentTC << "]\t" << absoluteErrorX << "\t" << absoluteErrorY << "\t" << relativeErrorX << "\t" << relativeErrorY << std::endl;
				
				currentTC++;
				if(currentTC == testCasesPerRegion) {
					testData.push_back(tc1);
					currentTC = 0;
					currentRegion++;
				}
				
				break;
			case 1:
				circle(testCanvas, Point(640 + 640 / 2, 540 / 2), 100, Scalar(0, 0, 255), 5);
				static vector<Point> tc2;
				tc2.push_back(out);
				currentTC++;
				
				if(currentTC == testCasesPerRegion) {
					testData.push_back(tc2);
					currentTC = 0;
					currentRegion++;
				}
				
				break;
			case 2:
				circle(testCanvas, Point(640 * 2 + 640 / 2, 540 / 2), 100, Scalar(0, 0, 255), 5);
				static vector<Point> tc3;
				tc3.push_back(out);
				currentTC++;
				
				if(currentTC == testCasesPerRegion) {
					testData.push_back(tc3);
					currentTC = 0;
					currentRegion++;
				}
				
				break;
			case 3:
				circle(testCanvas, Point(640/2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
				static vector<Point> tc4;
				tc4.push_back(out);
				currentTC++;
				
				if(currentTC == testCasesPerRegion) {
					testData.push_back(tc4);
					currentTC = 0;
					currentRegion++;
				}
				break;
			case 4:
				circle(testCanvas, Point(640 + 640 / 2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
				static vector<Point> tc5;
				tc5.push_back(out);
				currentTC++;
				
				if(currentTC == testCasesPerRegion) {
					testData.push_back(tc5);
					currentTC = 0;
					currentRegion++;
				}
				break;
			case 5:
				circle(testCanvas, Point(640 * 2 + 640 / 2, 540 + 540 / 2), 100, Scalar(0, 0, 255), 5);
				static vector<Point> tc6;
				tc5.push_back(out);
				currentTC++;
				
				if(currentTC == testCasesPerRegion) {
					testData.push_back(tc6);
					currentTC = 0;
					currentRegion++;
				}
				break;
			default:
				//TODO: implement function for calculate test results.
				break;
		}
	}

	return true;
}

void GazeEstimator::moduleDeinit(void)
{

}
