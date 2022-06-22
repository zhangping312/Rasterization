#pragma once

#include <Math/Vectors.h>

#include <string>
#include <functional>

namespace FW::Event
{
	class Event;
}

namespace FW::Window
{
	struct WindowProperty
	{
		std::string title;
		int width;
		int height;
		bool isWindowed;
	};

	using EventCallbackFunc = std::function<void(const Event::Event&)>;
	using ProcessUIEventFunc = std::function<void(const void*)>;

	class IWindow
	{
	protected:
		struct WindowData
		{
			std::string title;
			int width;
			int height;
			bool isWindowed = true;
			bool isResizing = false;

			EventCallbackFunc eventCallbackFunc;
			ProcessUIEventFunc uiProcessEventFunc;
		};
		WindowData m_WindowData;

	public:
		IWindow() = default;
		IWindow(const IWindow& rhs) = delete;
		IWindow& operator=(const IWindow& rhs) = delete;
		virtual ~IWindow() = default;

		virtual void Initialize(const WindowProperty& property, void* params = nullptr) = 0;
		virtual void Release() = 0;

		virtual void PollEvents() = 0;

		void SetEventCallbackFunc(EventCallbackFunc func) { m_WindowData.eventCallbackFunc = func; }
		void SetProcessUIEventFunc(ProcessUIEventFunc func) { m_WindowData.uiProcessEventFunc = func; }

		virtual void* GetNativeWindow() const = 0;
		const std::string& GetTitle() const { return m_WindowData.title; }
		int GetWidth() const { return m_WindowData.width; }
		int GetHeight() const { return m_WindowData.height; }
		float GetAspectRatio() const { return (float)m_WindowData.width / (float)m_WindowData.height; }
		virtual Math::Vector2 GetPosition() const = 0;
	};
}