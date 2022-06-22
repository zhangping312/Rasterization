#include "Level/Example/Example03_Interpolation.h"
#include "Graphics/RendererContext.h"
#include "Application.h"

#include <Logger/Logger.h>
#include <imgui.h>

using namespace FW::Math;

namespace Example::Example03
{
	void Rasterizer::BarycentricTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		Vector2 p0_raster = ToRaster(v0.position);
		Vector2 p1_raster = ToRaster(v1.position);
		Vector2 p2_raster = ToRaster(v2.position);

		// Check if triangle is backfacing.
		float area = EdgeFunction(p0_raster, p1_raster, p2_raster);
        if (area >= 0) return;

		Vector2 min, max;
		min.x = std::max(m_Viewport.x, std::min(p0_raster.x, std::min(p1_raster.x, p2_raster.x)));
		min.y = std::max(m_Viewport.y, std::min(p0_raster.y, std::min(p1_raster.y, p2_raster.y)));
		max.x = std::min(m_Viewport.z - 1.0f, std::max(p0_raster.x, std::max(p1_raster.x, p2_raster.x)));
		max.y = std::min(m_Viewport.w - 1.0f, std::max(p0_raster.y, std::max(p1_raster.y, p2_raster.y)));

		// Add 0.5 to sample at pixel centers.
        for (float x = min.x + 0.5f, xm = max.x + 0.5f; x <= xm; x += 1.0f)
		{
			for (float y = min.y + 0.5f, ym = max.y + 0.5f; y <= ym; y += 1.0f)
			{
				if (!IsInScissor(x, y)) continue;

				Vector2 p(x, y);

				float w0 = EdgeFunction(p1_raster, p2_raster, p) / area;
				float w1 = EdgeFunction(p2_raster, p0_raster, p) / area;
				float r = w0 * v0.color.r + w1 * v1.color.r + (1 - w0 - w1) * v2.color.r;
				float g = w0 * v0.color.g + w1 * v1.color.g + (1 - w0 - w1) * v2.color.g;
				float b = w0 * v0.color.b + w1 * v1.color.b + (1 - w0 - w1) * v2.color.b;
				float a = w0 * v0.color.a + w1 * v1.color.a + (1 - w0 - w1) * v2.color.a;
				auto color = Vector4(r, g, b, a);
				if (w0 >= 0 && w1 >= 0 && w0 + w1 <= 1) RendererContext::DrawPixel((int)x, (int)y, color);
			}
		}
	}

	void Rasterizer::EdgeFunctionTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		Vector2 p0_raster = ToRaster(v0.position);
		Vector2 p1_raster = ToRaster(v1.position);
		Vector2 p2_raster = ToRaster(v2.position);

		// Check if triangle is backfacing.
		float area = EdgeFunction(p0_raster, p1_raster, p2_raster);
        if (area >= 0) return;

		Vector2 min, max;
		min.x = std::max(m_Viewport.x, std::min(p0_raster.x, std::min(p1_raster.x, p2_raster.x)));
		min.y = std::max(m_Viewport.y, std::min(p0_raster.y, std::min(p1_raster.y, p2_raster.y)));
		max.x = std::min(m_Viewport.z - 1.0f, std::max(p0_raster.x, std::max(p1_raster.x, p2_raster.x)));
		max.y = std::min(m_Viewport.w - 1.0f, std::max(p0_raster.y, std::max(p1_raster.y, p2_raster.y)));

		// Add 0.5 to sample at pixel centers.
        for (float x = min.x + 0.5f, xm = max.x + 0.5f; x <= xm; x += 1.0f)
		{
			for (float y = min.y + 0.5f, ym = max.y + 0.5f; y <= ym; y += 1.0f)
			{
				if (!IsInScissor(x, y)) continue;

				Vector2 p(x, y);

				float w0 = EdgeFunction(p1_raster, p2_raster, p);
				float w1 = EdgeFunction(p2_raster, p0_raster, p);
				float w2 = EdgeFunction(p0_raster, p1_raster, p);

				Vector2 edge0 = p2_raster - p1_raster;
				Vector2 edge1 = p0_raster - p2_raster;
				Vector2 edge2 = p1_raster - p0_raster;

				bool overlaps = true;
				// If the point is on the edge, Test if it is a top or left edge,
				// otherwise Test if  the edge function is positive
				overlaps &= w0 == 0 ? IsTopLeft(p1_raster, p2_raster) : (w0 < 0);
				overlaps &= w1 == 0 ? IsTopLeft(p2_raster, p0_raster) : (w1 < 0);
				overlaps &= w2 == 0 ? IsTopLeft(p0_raster, p1_raster) : (w2 < 0);

				if (overlaps)
				{
					w0 /= area;
					w1 /= area;
					w2 /= area;

					float r = w0 * v0.color.r + w1 * v1.color.r + w2 * v2.color.r;
					float g = w0 * v0.color.g + w1 * v1.color.g + w2 * v2.color.g;
					float b = w0 * v0.color.b + w1 * v1.color.b + w2 * v2.color.b;
					float a = w0 * v0.color.a + w1 * v1.color.a + w2 * v2.color.a;
					auto color = Vector4(r, g, b, a);
					RendererContext::DrawPixel((int)x, (int)y, color);
				}
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

	float Rasterizer::EdgeFunction(const Vector2& v0, const Vector2 &v1, const Vector2 &p)
	{
		return Vector2::Cross(v1 - v0, p - v0);
	}

	bool Rasterizer::IsTopLeft(const FW::Math::Vector2& v0, const FW::Math::Vector2& v1)
	{
		Vector2 edge0 = v1 - v0;
		bool result = (edge0.y == 0 && edge0.x > 0) ||  edge0.y > 0;
		bool result1 = (v1.y == v0.y && v1.x > v0.x) || (v1.y > v0.y);
		return (v1.y == v0.y && v1.x > v0.x) || (v1.y > v0.y);
	}

	void Interpolation::Initialize()
	{
		auto& window = Application::Instance().GetWindowHandle();
		auto rect = Vector4(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}

	void Interpolation::Render()
	{
		RendererContext::SetClearColor(m_ClearColor);

		if (0 == m_SelectToggle) 	m_Rasterizer.BarycentricTriangle(m_Vertex0, m_Vertex1, m_Vertex2);
		else if (1 == m_SelectToggle) 	m_Rasterizer.EdgeFunctionTriangle(m_Vertex0, m_Vertex1, m_Vertex2);
	}

	void Interpolation::GUI()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::ColorEdit4("Clear Color", m_ClearColor.values);
		ImGui::DragFloat2("Position 0", m_Vertex0.position.values, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat2("Position 1", m_Vertex1.position.values, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat2("Position 2", m_Vertex2.position.values, 0.01f, -1.0f, 1.0f);
		ImGui::ColorEdit4("Color 0", m_Vertex0.color.values);
		ImGui::ColorEdit4("Color 1", m_Vertex1.color.values);
		ImGui::ColorEdit4("Color 2", m_Vertex2.color.values);
		if (ImGui::CollapsingHeader("Select mode"))
		{
			for (unsigned int index = 0; index < m_ToggleDescriptions.size(); ++index)
				ImGui::RadioButton(m_ToggleDescriptions[index].c_str(), &m_SelectToggle, index);
		}
		ImGui::End();
	}

	void Interpolation::OnWindowResize(const FW::Event::WindowResizeEvent& event)
	{
		auto rect = Vector4(0, 0, (float)event.GetWidth(), (float)event.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}
}