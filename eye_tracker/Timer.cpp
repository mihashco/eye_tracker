#include "Timer.h"
#include <iostream>

Timer::Timer()
{
	//
}


Timer::~Timer()
{
	//
}

void Timer::begin()
{
	this->start = high_resolution_clock::now();
}

void Timer::end()
{
	this->stop = high_resolution_clock::now();
	std::cout << "Elapsed milliseconds : [" << duration_cast<milliseconds>(this->stop - this->start).count() << "]" << std::endl;
}

int Timer::getElapsedMiliSeconds()
{
	this->stop = high_resolution_clock::now();
	return duration_cast<microseconds>(this->stop - this->start).count();
}