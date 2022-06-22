#pragma once

#include <Time/Time.h>
#include <Logger/Logger.h>
#include <Application/IApplication.h>

extern FW::Application::IApplication* CreateApplication();

int main(int argc, const char* argv[])
{
	FW::Logger::Initialize();
	FW::Time::Initialize();

	try
	{
		std::unique_ptr<FW::Application::IApplication> app(CreateApplication());
		while (app->IsRunning())
		{
			FW::Time::Update();
			app->Run();
		}
		app->Release();
	}
	catch (const std::exception& error)
	{
		LOG_ERROR(error.what());
		return EXIT_FAILURE;
	}

	FW::Logger::Release();

	return EXIT_SUCCESS;
}