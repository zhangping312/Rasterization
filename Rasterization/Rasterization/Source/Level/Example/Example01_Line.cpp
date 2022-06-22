#include "Level/Example/Example01_Line.h"
#include "Graphics/RendererContext.h"
#include "Application.h"

#include <Logger/Logger.h>
#include <imgui.h>

using namespace FW::Math;

namespace Example::Example01
{
	void Rasterizer::DDALine(const Vector2& p0, const Vector2& p1, const Vector4& color)
	{
		Vector2 p0_raster = ToRaster(p0);
		Vector2 p1_raster = ToRaster(p1);

		float dx = p1_raster.x - p0_raster.x;
		float dy = p1_raster.y - p0_raster.y;
		float step = std::abs(dx) >= std::abs(dy) ? std::abs(dx) : std::abs(dy);
		dx /= step;
		dy /= step;
		float x = p0_raster.x; float y = p0_raster.y;
		for (int i = 0; i < step; ++i)
		{
			if (IsInScissor(x, y)) RendererContext::DrawPixel((int)x, (int)y, color);
			x += dx;
			y += dy;
		}
	}

	void Rasterizer::BresenhamLine(const Vector2& p0, const Vector2& p1, const Vector4& color)
	{
		Vector2 p0_raster = ToRaster(p0);
		Vector2 p1_raster = ToRaster(p1);

		bool steep = false;
		float x0 = p0_raster.x, y0 = p0_raster.y;
		float x1 = p1_raster.x, y1 = p1_raster.y;
		if (std::abs(x0 - x1) < std::abs(y0 - y1))
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}
		if (x0 > x1) // Make it left to right
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		int dx = (int)(x1 - x0);
		int dy = (int)(y1 - y0);
		float derror2 = (float)(std::abs(dy) * 2);
		float error2 = 0.0f;
		int y = (int)y0;
		for (int x = (int)x0; x <= (int)x1; ++x)
		{
			if (steep)
			{
				if (IsInScissor((float)y, (float)x)) RendererContext::DrawPixel((int)y, (int)x, color);
			}
			else
			{
				if (IsInScissor((float)x, (float)y)) RendererContext::DrawPixel((int)x, (int)y, color);
			}
			error2 += derror2;
			if (error2 > dx)
			{
				y += (y1 > y0 ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}

	Vector2 Rasterizer::ToRaster(const Vector2& p_ndc)
	{
		float w = m_Viewport.x + (p_ndc.x + 1) * (m_Viewport.z - 1) / 2.0f;
		float h = m_Viewport.y + (p_ndc.y + 1) * (m_Viewport.w - 1) / 2.0f;
		return Vector2(w, h);
	}

	bool Rasterizer::IsInScissor(float w_raster, float h_raster)
	{
		return m_Scissor.x <= w_raster && w_raster <= m_Scissor.z && m_Scissor.y <= h_raster && h_raster <= m_Scissor.z;
	}

	void Line::Initialize()
	{
		auto& window = Application::Instance().GetWindowHandle();
		auto rect = Vector4(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}

	void Line::Render()
	{
		RendererContext::SetClearColor(m_ClearColor);

		if (m_IsDDALine) 	m_Rasterizer.DDALine(m_LinePoint1, m_LinePoint2, m_LineColor);
		else 				m_Rasterizer.BresenhamLine(m_LinePoint1, m_LinePoint2, m_LineColor);
	}

	void Line::GUI()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::ColorEdit4("Clear Color", m_ClearColor.values);
		ImGui::DragFloat2("Line Point 1", m_LinePoint1.values, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat2("Line Point 2", m_LinePoint2.values, 0.01f, -1.0f, 1.0f);
		ImGui::ColorEdit4("Line Color", m_LineColor.values);
		ImGui::Checkbox("Is DDA Line", &m_IsDDALine);
		ImGui::End();
	}

	void Line::OnWindowResize(const FW::Event::WindowResizeEvent& event)
	{
		auto rect = Vector4(0, 0, (float)event.GetWidth(), (float)event.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}
}