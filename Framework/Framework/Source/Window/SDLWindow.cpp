#include <Window/SDLWindow.h>
#include <Logger/Logger.h>
#include <Event/WindowEvent.h>

#include <SDL.h>

namespace FW::Window
{
	void SDLWindow::Initialize(const WindowProperty& property, void* params)
	{
		// Initialize SDL
		LOG_ASSERT(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL!");

		m_WindowData.title = property.title;
		m_WindowData.width = property.width;
		m_WindowData.height = property.height;
		m_WindowData.isWindowed = property.isWindowed;

		SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		m_NativeWindow = SDL_CreateWindow(m_WindowData.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowData.width, m_WindowData.height, flags);
		LOG_ASSERT(m_NativeWindow != nullptr, "Failed to create window!");
	}

	void SDLWindow::Release()
	{
		if (m_NativeWindow) SDL_DestroyWindow(m_NativeWindow);
		m_NativeWindow = nullptr;
		SDL_Quit();
	}

	void SDLWindow::PollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (m_WindowData.uiProcessEventFunc) m_WindowData.uiProcessEventFunc(&event);

			if (SDL_QUIT == event.type)
			{
				if (m_WindowData.eventCallbackFunc) m_WindowData.eventCallbackFunc(Event::WindowCloseEvent());
			}

			if (SDL_WINDOWEVENT == event.type && SDL_WINDOWEVENT_RESIZED == event.window.event)
			{
				m_WindowData.width = event.window.data1;
                m_WindowData.height = event.window.data2;
				Event::WindowResizeEvent event(m_WindowData.width, m_WindowData.height);
				if (m_WindowData.eventCallbackFunc) m_WindowData.eventCallbackFunc(event);
			}
		}
	}

	Math::Vector2 SDLWindow::GetPosition() const
	{
		int xPos = 0, yPos = 0;
		if (m_NativeWindow) SDL_GetWindowPosition(m_NativeWindow, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}
}