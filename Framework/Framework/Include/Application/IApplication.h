#pragma once

#include <Event/WindowEvent.h>

namespace FW::Application
{
	class IApplication
	{
	public:
		IApplication() = default;
		IApplication(const IApplication& rhs) = delete;
		IApplication& operator=(const IApplication& rhs) = delete;
		virtual ~IApplication() = default;

		virtual void Initialize(const std::string& title, int width, int height, bool isWindowed = true) = 0;
		virtual void Release() = 0;

		virtual void Run() = 0;
		virtual void Quit() = 0;

		virtual bool IsRunning() const = 0;

	protected:
		virtual void OnEvent(const Event::Event& event) = 0;
	};
};