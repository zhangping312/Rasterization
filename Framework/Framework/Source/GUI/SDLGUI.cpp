#include <GUI/SDLGUI.h>
#include <Logger/Logger.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl.cpp>
#include <backends/imgui_impl_sdlrenderer.cpp>

namespace FW::GUI
{
	void SDLGUI::Initialize(const Window::IWindow& window, const GuiProperty& property)
	{
		auto& imguiProperty = *(ImguiProperty*)(GuiProperty*)&property;
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = imguiProperty.isMoveFromTitleBarOnly;
		// if (imguiProperty.isDockingEnable)		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		// if (imguiProperty.isViewportsEnable) 	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	// Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		auto* nativeWindow = (SDL_Window*)window.GetNativeWindow();
		auto* renderer = SDL_GetRenderer(nativeWindow);
		LOG_ASSERT(renderer != nullptr, "Can't get the renderer associated with the window!")
		ImGui_ImplSDL2_InitForSDLRenderer(nativeWindow, renderer);
		ImGui_ImplSDLRenderer_Init(renderer);
	}

	void SDLGUI::Release()
	{
		// Cleanup
		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void SDLGUI::ProcessEvent(const void* event)
	{
		ImGui_ImplSDL2_ProcessEvent((const SDL_Event*)event);
	}

	void SDLGUI::BeginRender()
	{
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
	}

	void SDLGUI::EndRender()
	{
		// Rendering
		ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}
}