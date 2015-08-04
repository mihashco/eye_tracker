#ifndef __HEAD_DETECTOR_H__
#define __HEAD_DETECTOR_H__

#include "AppModule.h"
#include "Kalman.h"

class HeadDetector : public AppModule
{
private:
	Kalman kFilter;
public:
	HeadDetector();
	~HeadDetector();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
	void computeHeatMap(ApplicationState &appState);
};

#endif

