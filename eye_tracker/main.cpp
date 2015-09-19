#include "opencv2/opencv.hpp"
#include "EyeTracker.h"

#include "HoughDetector.h"
#include "ThresholdDetector.h"
#include "MinDetector.h"
#include "Timer.h"

using namespace cv;
using namespace std;


int main(int argc, char **argv)
{
	EyeTracker eyeTrackerApplication(argc, argv);
	eyeTrackerApplication.applicationModulesInit();
	eyeTrackerApplication.startApplicationLoop();
	eyeTrackerApplication.applicationModulesDeinit();
	
	
	HoughDetector hDetector;
	ThresholdDetector tDetector;
	MinDetector mDetector;

	//20 LUX
	Point reference(38, 18); //TOP
	//Point reference(44, 23); //BOTTOM
	//Point reference(36, 19);  //CENTER
	//Point reference(41, 17); //LEFT
	//Point reference(27, 18); //RIGTH

	//130 LUX
	//Point reference(42, 22); //TOP
	//Point reference(43, 24); //BOTTOM
	//Point reference(40, 18);  //CENTER
	//Point reference(35, 18); //LEFT
	//Point reference(45, 19); //RIGTH

	//320 LUX
	//Point reference(34, 16); //TOP
	//Point reference(38, 22); //BOTTOM
	//Point reference(39, 18);  //CENTER
	//Point reference(33, 16); //LEFT
	//Point reference(38, 18); //RIGTH

	//int valid = 0;
	//int invalid = 0;
	//Timer t;

	//for (int i = 0; i < 100; i++) {
	//	stringstream ss;
	//	stringstream fileSaveNameStream;
	//	ss << "C:\\Users\\mszcz_000\\Desktop\\eye_test_images\\320_lx_top\\test_" << i << ".jpg";
	//	fileSaveNameStream << "reusult_threshold_" << i << ".jpg";

	//	Mat img = imread(ss.str(), 1);

	//	cvtColor(img, img, CV_BGR2GRAY);

	//	t.begin();
	//	Point r1;
	//	Point r2;

	//	int distX;
	//	int distY;

	//	Point d = tDetector.detect(img);//, r1, r2, distX, distY);
	//	std::cout << t.getElapsedMiliSeconds() << std::endl;
	//	std::cout << "DETECTION : " << d << std::endl;
	//
	//	circle(img, d, 10, Scalar(255, 255, 255), 2);

	//	int dx = abs(d.x - reference.x);
	//	int dy = abs(d.y - reference.y);

	//	//circle(img, d, 10, Scalar(255, 255, 255), 2);
	//	imwrite(fileSaveNameStream.str(), img);
	//	 
	//	if (dx <= 6 && dy <= 4)
	//		valid++;
	//	else
	//		invalid++;

	//	imshow("Debug1", img);
	//	waitKey(5);
	//}

	//std::cout << "===============================TEST RESULT=====================================" << std::endl;
	//std::cout << "VALID : " << valid << std::endl;
	//std::cout << "INVALID :" << invalid << std::endl;

	//system("pause");
	//
	//return 0;
}