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
}


GazeEstimator::~GazeEstimator()
{
}

void GazeEstimator::moduleInit(ApplicationState &appState)
{
	std::cout << "GAZE module init" << std::endl;
	this->heatMap = Mat::zeros(Size(1920, 1080), CV_8UC3);
}

bool GazeEstimator::moduleProcess(ApplicationState &appState)
{
	gazeCanvas = Mat::zeros(Size(1920, 1080), CV_8UC3);
	double usedFactor;

	if (appState.eyeDeltaX > 0) {
		usedFactor = appState.rightFactor;
	}
	else {
		usedFactor = appState.leftFactor;
	}
	
	Point out(appState.headCenterPoint.x * 1.5 - appState.eyeDeltaX / usedFactor, appState.headCenterPoint.y * 1.5 - appState.eyeDeltaY);
	circle(gazeCanvas, out, 200, Scalar(255, 0, 0), 3);

	if (appState.gzMode == GAZE_DEBUG) {
		warpAffine(gazeCanvas, gazeCanvas, this->rotation, gazeCanvas.size());
		flip(gazeCanvas, gazeCanvas, 0);
		std::cout << out << std::endl;
		imshow("GAZE_DEBUG", gazeCanvas);
	}

	return true;
}

void GazeEstimator::moduleDeinit(void)
{

}
