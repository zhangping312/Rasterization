#pragma once

#include <string>
#include <functional>

namespace FW::Event
{
	enum class EVENT_TYPE
	{
		None = 0,
		WindowClose, WindowPause, WindowResize
	};

	#define EVENT_CLASS_TYPE(type) 	static EVENT_TYPE GetStaticType() { return EVENT_TYPE::type; }\
									virtual EVENT_TYPE GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

	class Event
	{
	public:
		Event() = default;
		Event(const Event& rhs) = delete;
		Event& operator=(const Event& rhs) = delete;
		virtual ~Event() = default;

		virtual EVENT_TYPE GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		friend std::ostream& operator<<(std::ostream& os, const Event& event)
		{
			return os << event.ToString();
		}
	};

	class EventDispatcher
	{
	private:
		template<typename T>
		using EventFunc = std::function<void(const T&)>;

		const Event& m_Event;

	public:
		EventDispatcher(const Event& event) : m_Event(event) {}
		EventDispatcher(const EventDispatcher& rhs) = delete;
		EventDispatcher& operator=(const EventDispatcher& rhs) = delete;
		~EventDispatcher() = default;

		template<typename T>
		void Dispatch(EventFunc<T> func)
		{
			if (T::GetStaticType() == m_Event.GetEventType()) func(*(T*)&m_Event);
		}
	};
}