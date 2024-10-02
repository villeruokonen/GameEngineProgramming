#include "TheApp.h"

int APIENTRY WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow)
{
	auto app = std::make_unique<TheApp>();
	if(!app->Create(1280, 720, "GAME"))
	{
		IApplication::Debug("APP START FAILED\n");
		return -1;
	}

	app->Run();
	return 0;
}
