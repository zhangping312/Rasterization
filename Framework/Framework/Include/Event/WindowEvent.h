#pragma once

#include <Event/Event.h>

#include <sstream>

namespace FW::Event
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowPauseEvent : public Event
	{
	private:
		bool m_IsPaused;

	public:
		WindowPauseEvent(bool isPaused) : m_IsPaused(isPaused) {}

		bool IsPaused() const { return m_IsPaused; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << (m_IsPaused ? "Paused" : "Active");
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowPause)
	};

	class WindowResizeEvent : public Event
	{
	private:
		int m_Width, m_Height;

	public:
		WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
	};
}