#ifndef __HEAD_DETECTOR_H__
#define __HEAD_DETECTOR_H__

#include "AppModule.h"

class HeadDetector : public AppModule
{
private:
public:
	HeadDetector();
	~HeadDetector();

	void moduleInit(ApplicationState &appState);
	bool moduleProcess(ApplicationState &appState);
	void moduleDeinit(void);
};

#endif

