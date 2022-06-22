#include "Graphics/RendererContext.h"

std::unique_ptr<Renderer> RendererContext::s_Renderer = nullptr;

void RendererContext::Initialize(FW::Window::IWindow* windowHandle)
{
	s_Renderer = std::make_unique<Renderer>();
	s_Renderer->Initialize(windowHandle);
}

void RendererContext::Release()
{
	if (s_Renderer) s_Renderer->Release();
}

void RendererContext::SetClearColor(const FW::Math::Vector4& clearColor)
{
	if (s_Renderer) s_Renderer->SetClearColor(clearColor);
}

void RendererContext::Clear()
{
	if (s_Renderer) s_Renderer->Clear();
}

float RendererContext::GetDepth(int x, int y)
{
	if (s_Renderer) return s_Renderer->GetDepth(x, y);
	return 0.0f;
}

void RendererContext::SetDepth(int x, int y, float depth)
{
	if (s_Renderer) s_Renderer->SetDepth(x, y, depth);
}

void RendererContext::DrawPixel(int x, int y, const FW::Math::Vector4& color)
{
	if (s_Renderer) s_Renderer->DrawPixel(x, y, color);
}

void RendererContext::DrawPixel(int x, int y, uint32_t color)
{
	if (s_Renderer) s_Renderer->DrawPixel(x, y, color);
}

void RendererContext::Present()
{
	if (s_Renderer) s_Renderer->Present();
}

void RendererContext::SwapBuffers()
{
	if (s_Renderer) s_Renderer->SwapBuffers();
}

void RendererContext::OnWindowResize(int width, int height)
{
	if (s_Renderer) s_Renderer->OnWindowResize(width, height);
}