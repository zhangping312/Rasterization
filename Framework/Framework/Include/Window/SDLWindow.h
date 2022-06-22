#pragma once

#include <Window/IWindow.h>

struct SDL_Window;

namespace FW::Window
{
	class SDLWindow : public IWindow
	{
	private:
		SDL_Window* m_NativeWindow = nullptr;

	public:
		virtual void Initialize(const WindowProperty& property, void* params = nullptr) override;
		virtual void Release() override;

		virtual void PollEvents() override;

		virtual void* GetNativeWindow() const override { return (void*)m_NativeWindow; }
		virtual Math::Vector2 GetPosition() const override;
	};
}