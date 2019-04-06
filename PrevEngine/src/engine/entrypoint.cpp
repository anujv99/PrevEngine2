#include "pch.h"

#include "application.h"

using namespace prev;

extern prev::Application * CreateApplication();

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	prev::Application * app = CreateApplication();

	if (!app->IsAppReady) {
		PV_POST_FATAL("Unable to initialize engine");
		delete app;
		return -1;
	}

	if (app != nullptr)
		app->Run();

	delete app;

	return 0;
}