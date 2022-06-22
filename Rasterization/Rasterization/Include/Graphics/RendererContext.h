#pragma once

#include "Graphics/Renderer.h"

#include <memory>

class RendererContext
{
private:
	static std::unique_ptr<Renderer> s_Renderer;

public:
	static void Initialize(FW::Window::IWindow* windowHandle);
	static void Release();

	static void SetClearColor(const FW::Math::Vector4& clearColor);
	static void Clear();

	static float GetDepth(int x, int y);
	static void SetDepth(int x, int y, float depth);

	static void DrawPixel(int x, int y, const FW::Math::Vector4& color);
	static void DrawPixel(int x, int y, uint32_t color);

	static void Present();
	static void SwapBuffers();

	static void OnWindowResize(int width, int height);
};