#include "Level/Example/Example02_Triangle.h"
#include "Graphics/RendererContext.h"
#include "Application.h"

#include <Logger/Logger.h>
#include <imgui.h>

using namespace FW::Math;

namespace Example::Example02
{
	void Rasterizer::ScanLineTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vector4& color)
	{
		Vector2 p0_raster = ToRaster(v0.position);
		Vector2 p1_raster = ToRaster(v1.position);
		Vector2 p2_raster = ToRaster(v2.position);

		// Check if triangle is backfacing.
		float area = EdgeFunction(p0_raster, p1_raster, p2_raster);
        if (area >= 0) return;

		// Sort the vertices
		if (p0_raster.y > p1_raster.y) std::swap(p0_raster, p1_raster);
		if (p0_raster.y > p2_raster.y) std::swap(p0_raster, p2_raster);
		if (p1_raster.y > p2_raster.y) std::swap(p1_raster, p2_raster);

		int totalHeight = (int)(p2_raster.y - p0_raster.y);
		for (int height = 0; height < totalHeight; ++height)
		{
			bool isSecond = height > (p1_raster.y - p0_raster.y) || p0_raster.y == p1_raster.y;
			int segmentHeight = (int)(isSecond ? p2_raster.y - p1_raster.y : p1_raster.y - p0_raster.y);
			float alpha = (float)height / totalHeight;
			float betaHeight = height - (isSecond ? p1_raster.y - p0_raster.y : 0);
			float beta  = betaHeight / segmentHeight;
			float ax = p0_raster.x + (p2_raster.x - p0_raster.x) * alpha;
			float bx = isSecond ? p1_raster.x + (p2_raster.x - p1_raster.x) * beta : p0_raster.x + (p1_raster.x - p0_raster.x) * beta;
			if (ax > bx) std::swap(ax, bx);
			for (float x = ax; x <= bx; ++x)
			{
				if (IsInScissor(x, p0_raster.y + height)) RendererContext::DrawPixel((int)x, (int)(p0_raster.y + height), color);
			}
		}
	}

	void Rasterizer::BarycentricTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vector4& color)
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

				float u = EdgeFunction(p1_raster, p2_raster, p) / area;
				float v = EdgeFunction(p2_raster, p0_raster, p) / area;
				if (u >= 0 && v >= 0 && u + v <= 1) RendererContext::DrawPixel((int)x, (int)y, color);
			}
		}
	}

	void Rasterizer::EdgeFunctionTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vector4& color)
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

				bool overlaps = w0 < 0 && w1 < 0 && w2 < 0;

				if (overlaps) RendererContext::DrawPixel((int)x, (int)y, color);
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

	void Triangle::Initialize()
	{
		auto& window = Application::Instance().GetWindowHandle();
		auto rect = Vector4(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}

	void Triangle::Render()
	{
		RendererContext::SetClearColor(m_ClearColor);

		if (0 == m_SelectToggle) 		m_Rasterizer.ScanLineTriangle(m_Vertex0, m_Vertex1, m_Vertex2, m_TriangleColor);
		else if (1 == m_SelectToggle) 	m_Rasterizer.BarycentricTriangle(m_Vertex0, m_Vertex1, m_Vertex2, m_TriangleColor);
		else if (2 == m_SelectToggle) 	m_Rasterizer.EdgeFunctionTriangle(m_Vertex0, m_Vertex1, m_Vertex2, m_TriangleColor);
	}

	void Triangle::GUI()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::ColorEdit4("Clear Color", m_ClearColor.values);
		ImGui::DragFloat2("Position 0", m_Vertex0.position.values, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat2("Position 1", m_Vertex1.position.values, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat2("Position 2", m_Vertex2.position.values, 0.01f, -1.0f, 1.0f);
		ImGui::ColorEdit4("Triangle Color", m_TriangleColor.values);
		if (ImGui::CollapsingHeader("Select mode"))
		{
			for (unsigned int index = 0; index < m_ToggleDescriptions.size(); ++index)
				ImGui::RadioButton(m_ToggleDescriptions[index].c_str(), &m_SelectToggle, index);
		}
		ImGui::End();
	}

	void Triangle::OnWindowResize(const FW::Event::WindowResizeEvent& event)
	{
		auto rect = Vector4(0, 0, (float)event.GetWidth(), (float)event.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}
}