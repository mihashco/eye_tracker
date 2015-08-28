#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>

using namespace std;
using namespace chrono;

class Timer
{
private:
	high_resolution_clock::time_point start;
	high_resolution_clock::time_point stop;

public:
	Timer();
	~Timer();

	void begin(void);
	void end(void);
	int getElapsedMiliSeconds(void);
};

#endif
