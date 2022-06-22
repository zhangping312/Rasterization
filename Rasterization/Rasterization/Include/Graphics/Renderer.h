#pragma once

#include <Window/IWindow.h>
#include <Math/Vectors.h>
#include <SDL.h>
#include <stdint.h>

class Renderer
{
private:
	SDL_Renderer* m_NativeRenderer = nullptr;
	// Store the color buffer, used to render
	SDL_Texture* m_Texture = nullptr;

	struct FrameBuffer
	{
		int width = 0;
		int height = 0;
		uint32_t* colorBuffer = nullptr;
		float* depthBuffer = nullptr;
	};
	FrameBuffer m_FrameBuffer;
	uint32_t m_ClearColor;

public:
	Renderer() = default;
	Renderer(const Renderer& rhs) = delete;
	Renderer& operator=(const Renderer& rhs) = delete;
	~Renderer() = default;

	void Initialize(FW::Window::IWindow* windowHandle);
	void Release();

	void SetClearColor(const FW::Math::Vector4& clearColor);
	void Clear();

	float GetDepth(int x, int y) const;
	void SetDepth(int x, int y, float depth);

	void DrawPixel(int x, int y, const FW::Math::Vector4& color);
	void DrawPixel(int x, int y, uint32_t color);

	void Present();
	void SwapBuffers();

	void OnWindowResize(int width, int height);

private:
	void CreateResource(int width, int height);
	void DeleteResource();
};