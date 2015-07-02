#include "AppModule.h"

class FramePreprocessor : public AppModule
{
public:
	FramePreprocessor();
	~FramePreprocessor();

	void moduleInit(ApplicationState &appState);
	void moduleDeinit();
	bool moduleProcess(ApplicationState &appState);
};

