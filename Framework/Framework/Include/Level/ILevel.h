#pragma once

#include <Event/WindowEvent.h>

#include <string>

namespace FW::Level
{
	#define LEVEL_CLASS_TYPE(type) 	static std::string GetLevelName() { return #type; }\
									virtual std::string GetName() const override { return GetLevelName(); }

	class ILevel
	{
	public:
		ILevel() = default;
		ILevel(const ILevel& rhs) = delete;
		ILevel& operator=(const ILevel& rhs) = delete;
		virtual ~ILevel() = default;

		virtual void Initialize() {}
		virtual void Release() {}

		virtual void Update() {}
		virtual void Render() = 0;
		virtual void GUI() {}

		virtual void OnWindowResize(const FW::Event::WindowResizeEvent& event) {}

		virtual std::string GetName() const = 0;
	};
}