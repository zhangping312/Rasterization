#include "Application.h"
#include "Graphics/RendererContext.h"

#include <Logger/Logger.h>
#include <Window/SDLWindow.h>
#include <GUI/SDLGUI.h>
#include <Level/LevelManager.h>

using namespace FW::Level;

Application* Application::s_Instance = nullptr;

void Application::Initialize(const std::string& title, int width, int height, bool isWindowed)
{
	LOG_ASSERT(!s_Instance, "Application already exists!")
	s_Instance = this;

	// Create window
	m_WindowHandle = std::make_unique<FW::Window::SDLWindow>();
	m_WindowHandle->Initialize({ .title = title, .width = width, .height = height, .isWindowed = isWindowed });
	m_WindowHandle->SetEventCallbackFunc(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	// Create renderer
	RendererContext::Initialize(m_WindowHandle.get());
	// Create gui
	m_GUI = std::make_unique<FW::GUI::SDLGUI>();
	FW::GUI::ImguiProperty guiProperty { .isMoveFromTitleBarOnly = true, .isDockingEnable = true, .isViewportsEnable = true };
	m_GUI->Initialize(*m_WindowHandle.get(), guiProperty);
	m_WindowHandle->SetProcessUIEventFunc(std::bind(&FW::GUI::IGUI::ProcessEvent, m_GUI.get(), std::placeholders::_1));

	// Create level editor
	m_LevelEditor = std::make_unique<LevelEditor>();

	m_IsRunning = true;
}

void Application::Release()
{
	if (m_GUI) m_GUI->Release();
	RendererContext::Release();
	if (m_WindowHandle) m_WindowHandle->Release();
	s_Instance = nullptr;
}

void Application::Run()
{
	m_WindowHandle->PollEvents();
	Update();
	Render();
}

void Application::Quit()
{
	m_IsRunning = false;
}

void Application::OnEvent(const FW::Event::Event& event)
{
	FW::Event::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<FW::Event::WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
	dispatcher.Dispatch<FW::Event::WindowPauseEvent>(std::bind(&Application::OnWindowPause, this, std::placeholders::_1));
	dispatcher.Dispatch<FW::Event::WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
}

void Application::OnWindowClose(const FW::Event::WindowCloseEvent& event)
{
	m_IsRunning = false;
}

void Application::OnWindowPause(const FW::Event::WindowPauseEvent& event)
{
	LOG_INFO(event.ToString());
}

void Application::OnWindowResize(const FW::Event::WindowResizeEvent& event)
{
	if (0 == event.GetWidth() || 0 == event.GetHeight()) return;
	RendererContext::OnWindowResize(event.GetWidth(), event.GetHeight());
	auto& levels = LevelManager::Instane().GetActiveLevels();
	for (auto& iter : levels) iter.second->OnWindowResize(event);
}

void Application::Update()
{
	auto& levels = LevelManager::Instane().GetActiveLevels();
	for (auto& iter : levels) iter.second->Update();
}

void Application::Render()
{
	RendererContext::Clear();
	// Render levels
	auto& levels = LevelManager::Instane().GetActiveLevels();
	for (auto& iter : levels) iter.second->Render();
	RendererContext::Present();
	// Render gui
	if (m_GUI) m_GUI->BeginRender();
	for (auto& iter : levels) iter.second->GUI();
	if (m_LevelEditor) m_LevelEditor->GUI();
	if (m_GUI) m_GUI->EndRender();
	// Present
	RendererContext::SwapBuffers();
}