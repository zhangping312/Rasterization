#pragma once

#include <Window/IWindow.h>

namespace FW::GUI
{
	struct GuiProperty
	{
	};

	struct ImguiProperty : public GuiProperty
	{
		bool isMoveFromTitleBarOnly;
		bool isDockingEnable;
		bool isViewportsEnable;
	};

	class IGUI
	{
	public:
		IGUI() = default;
		IGUI(const IGUI& rhs) = delete;
		IGUI& operator=(const IGUI& rhs) = delete;
		virtual ~IGUI() = default;

		virtual void Initialize(const Window::IWindow& window, const GuiProperty& property) = 0;
		virtual void Release() = 0;

		virtual void ProcessEvent(const void* event) = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;
	};
}