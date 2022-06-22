#pragma once

#include "Editor/LevelEditor.h"

#include <Application/IApplication.h>
#include <Window/IWindow.h>
#include <Event/WindowEvent.h>
#include <GUI/IGUI.h>

class Application : public FW::Application::IApplication
{
private:
	static Application* s_Instance;
	bool m_IsRunning;
	std::unique_ptr<FW::Window::IWindow> m_WindowHandle;
	std::unique_ptr<FW::GUI::IGUI> m_GUI;
	std::unique_ptr<LevelEditor> m_LevelEditor;

public:
	virtual void Initialize(const std::string& title, int width, int height, bool isWindowed = true) override;
	virtual void Release() override;

	virtual void Run() override;
	virtual void Quit() override;

	virtual bool IsRunning() const override { return m_IsRunning; }

	static Application& Instance() { return *s_Instance; }
	FW::Window::IWindow& GetWindowHandle() const { return *m_WindowHandle; }

protected:
	virtual void OnEvent(const FW::Event::Event& event) override;
	void OnWindowClose(const FW::Event::WindowCloseEvent& event);
	void OnWindowPause(const FW::Event::WindowPauseEvent& event);
	void OnWindowResize(const FW::Event::WindowResizeEvent& event);

private:
	void Update();
	void Render();
};