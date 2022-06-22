#include "Application.h"

#include <Main.h>

#define APP_NAME "Rasterization"
#define APP_WIDTH 1280
#define APP_HEIGHT 720

FW::Application::IApplication* CreateApplication()
{
	auto* app = new Application();
	app->Initialize(APP_NAME, APP_WIDTH, APP_HEIGHT);
	return app;
}