#include "Calibrator.h"

Calibrator::Calibrator()
{
	//This should read screen size!
	this->left.x = 0;
	this->left.y = 540;

	this->right.x = 1920;
	this->right.y = 540;

	this->bottom.x = 960;
	this->bottom.y = 1080;

	this->top.x = 960;
	this->top.y = 0;

	this->center.x = 960;
	this->center.y = 540;

	cv::namedWindow("CalibratorWin");
	cv::resizeWindow("CalibratorWin", 100, 100);
}

Calibrator::~Calibrator()
{
	//
}

bool Calibrator::moduleProcess(int x, int y)
{
	static int ticks = 0;
	ticks++;

	// std::cout << ticks << " " << x << " y " << y << std::endl;

	if(ticks > 91)
	{
		return true;
	}

	//get coordinates of the eye if user looks at the center point
	if(ticks < 15)
	{
		cv::moveWindow("CalibratorWin", this->center.x, this->center.y);
		centerV.push_back(Point(x,y));
	}
	else if(ticks > 15 && ticks < 30)
	{
		cv::moveWindow("CalibratorWin", this->left.x, this->left.y);
		leftV.push_back(Point(x,y));
	}
	else if(ticks > 45 && ticks < 60)
	{
		cv::moveWindow("CalibratorWin", this->right.x, this->right.y);
		rightV.push_back(Point(x,y));
	}
	else if(ticks > 60 && ticks < 75)
	{
		cv::moveWindow("CalibratorWin", this->bottom.x, this->bottom.y);
		bottomV.push_back(Point(x,y));
	}
	else if(ticks > 75 && ticks < 90)
	{
		cv::moveWindow("CalibratorWin", this->top.x, this->top.y);
		topV.push_back(Point(x,y));
	}

	//calibration data aquisition is ready so user is able to 
	//compute dx and dy values.
	if(ticks == 90)
	{
		computeDeltaXY();
	}

	return false;
}

void Calibrator::computeDeltaXY(void)
{
	//get mean value for each vector

	//skoro dla srodka wspolrzedne maja xc , yc;
	//a dla lewego punkty xl yl - odliczam roznice dx i dy.

	int cx = 0;
	int lx = 0;


	//get the dx from center and left
	for(int i = 0 ; i < 15; i++)
	{
		cx += centerV[i].x;
		lx += leftV[i].x;
	}

	cx /= 15;
	lx /= 15;

	int dx = abs(cx - lx);

	int cy = 0;
	int ly = 0;

	for(int i = 0 ; i < 15; i++)
	{
		cy += centerV[i].y;
		ly += leftV[i].y;
	}

	cy /= 15;
	ly /= 15;

	int dy = abs(cy - ly);

	mvX = 960/dx;
	mvY = 540/dy;
}

void Calibrator::moveCursor(int x, int y)
{
	cv::moveWindow("CalibratorWin", x * mvX, y * mvY);
	std::cout << "MoveX [" << x*mvX << "] [" << y * mvY << "]" << std::endl;
}