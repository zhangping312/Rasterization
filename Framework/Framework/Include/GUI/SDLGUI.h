#pragma once

#include <GUI/IGUI.h>

namespace FW::GUI
{
	class SDLGUI : public IGUI
	{
	public:
		virtual void Initialize(const Window::IWindow& window, const GuiProperty& property) override;
		virtual void Release() override;

		virtual void ProcessEvent(const void* event) override;

		virtual void BeginRender() override;
		virtual void EndRender() override;
	};
}