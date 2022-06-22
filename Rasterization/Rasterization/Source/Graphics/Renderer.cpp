#include "Graphics/Renderer.h"
#include "Graphics/RendererUtility.h"

#include <Logger/Logger.h>

void Renderer::Initialize(FW::Window::IWindow* windowHandle)
{
	// Create sdl renderer - used to render color buffer and gui
	auto* nativeWindow = (SDL_Window*)windowHandle->GetNativeWindow();
	auto flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
	m_NativeRenderer = SDL_CreateRenderer(nativeWindow, -1, flags);
	LOG_ASSERT(m_NativeRenderer != nullptr, "Failed to create SDL renderer!");

	// Create buffer and texture
	CreateResource(windowHandle->GetWidth(), windowHandle->GetHeight());
}

void Renderer::Release()
{
	DeleteResource();

	if (m_NativeRenderer) SDL_DestroyRenderer(m_NativeRenderer);
}

void Renderer::SetClearColor(const FW::Math::Vector4& clearColor)
{
	m_ClearColor = FormatColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void Renderer::Clear()
{
	LOG_ASSERT(m_FrameBuffer.colorBuffer != nullptr, "Color buffer used before created!");
	int index = 0;
	for (int y = 0; y < m_FrameBuffer.height; ++y)
	{
		for (int x = 0; x < m_FrameBuffer.width; ++x)
		{
			index = y * m_FrameBuffer.width + x;
			m_FrameBuffer.colorBuffer[index] = m_ClearColor;
		}
	}
	LOG_ASSERT(m_FrameBuffer.depthBuffer != nullptr, "Depth buffer used before created!");
	index = 0;
	for (int y = 0; y < m_FrameBuffer.height; ++y)
	{
		for (int x = 0; x < m_FrameBuffer.width; ++x)
		{
			index = y * m_FrameBuffer.width + x;
			m_FrameBuffer.depthBuffer[index] = std::numeric_limits<float>::max();
		}
	}
}

float Renderer::GetDepth(int x, int y) const
{
	LOG_ASSERT(m_FrameBuffer.depthBuffer != nullptr, "Depth buffer used before created!");
	int index = y * m_FrameBuffer.width + x;
	return m_FrameBuffer.depthBuffer[index];
}

void Renderer::SetDepth(int x, int y, float depth)
{
	LOG_ASSERT(m_FrameBuffer.depthBuffer != nullptr, "Depth buffer used before created!");
	int index = y * m_FrameBuffer.width + x;
	m_FrameBuffer.depthBuffer[index] = depth;
}

void Renderer::DrawPixel(int x, int y, const FW::Math::Vector4& color)
{
	// LOG_ASSERT(m_FrameBuffer.colorBuffer != nullptr, "Color buffer used before created!");
	// LOG_ASSERT(0 <= x && x < m_FrameBuffer.width && 0 <= y && y < m_FrameBuffer.height, "Invalid position");

	// int index = y * m_FrameBuffer.width + x;
	// m_FrameBuffer.colorBuffer[index] = FormatColor(color.r, color.g, color.b, color.a);
	DrawPixel(x, y, FormatColor(color.r, color.g, color.b, color.a));
}

void Renderer::DrawPixel(int x, int y, uint32_t color)
{
	LOG_ASSERT(m_FrameBuffer.colorBuffer != nullptr, "Color buffer used before created!");
	LOG_ASSERT(0 <= x && x < m_FrameBuffer.width && 0 <= y && y < m_FrameBuffer.height, "Invalid position");

	int index = y * m_FrameBuffer.width + x;
	m_FrameBuffer.colorBuffer[index] = color;
}

void Renderer::Present()
{
	if (!m_Texture || !m_NativeRenderer) return;

	// Copy color buffer to sdl texture
	void* pixels;
	int pitch = 0;
	SDL_LockTexture(m_Texture, nullptr, &pixels, &pitch);
	memcpy(pixels, m_FrameBuffer.colorBuffer, m_FrameBuffer.height * pitch);
	SDL_UnlockTexture(m_Texture);

	SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
	SDL_RenderCopyEx(m_NativeRenderer, m_Texture, nullptr, nullptr, 0, nullptr, flip);
}

void Renderer::SwapBuffers()
{
	if (m_NativeRenderer) SDL_RenderPresent(m_NativeRenderer);
}

void Renderer::OnWindowResize(int width, int height)
{
	CreateResource(width, height);
}

void Renderer::CreateResource(int width, int height)
{
	if (m_FrameBuffer.width != width && m_FrameBuffer.height != height)
	{
		DeleteResource();

		m_FrameBuffer.width = width;
		m_FrameBuffer.height = height;
		int size = m_FrameBuffer.width * m_FrameBuffer.height;
		m_FrameBuffer.colorBuffer = new uint32_t[size];
		m_FrameBuffer.depthBuffer = new float[size];
		// Create sdl texture
		m_Texture = SDL_CreateTexture(m_NativeRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	}
}

void Renderer::DeleteResource()
{
	// Delete old color buffer
	if (m_FrameBuffer.colorBuffer != nullptr) delete[] m_FrameBuffer.colorBuffer;
	m_FrameBuffer.colorBuffer = nullptr;
	// Delete old depth buffer
	if (m_FrameBuffer.depthBuffer != nullptr) delete[] m_FrameBuffer.depthBuffer;
	m_FrameBuffer.depthBuffer = nullptr;

	// Delete sdl texture
	if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
	m_Texture = nullptr;
}