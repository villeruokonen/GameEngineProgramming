
#include "BowlingApp.h"

int APIENTRY WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow)
{
	auto app = std::make_unique<BowlingApp>();
	if (!app->Create(1280, 720, "BOWLING!"))
	{
		IApplication::Debug("APP START FAILED");
		return 0;
	}

	app->Run();
	return 0;
}

