#include "Level/Example/Example04_Index.h"
#include "Graphics/RendererContext.h"
#include "Application.h"

#include <Logger/Logger.h>
#include <imgui.h>

using namespace FW::Math;

namespace Example::Example04
{
	void Rasterizer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
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
				overlaps &= (w0 == 0 ? ((edge0.y == 0 && edge0.x > 0) ||  edge0.y > 0) : (w0 < 0));
				overlaps &= (w1 == 0 ? ((edge1.y == 0 && edge1.x > 0) ||  edge1.y > 0) : (w1 < 0));
				overlaps &= (w2 == 0 ? ((edge2.y == 0 && edge2.x > 0) ||  edge2.y > 0) : (w2 < 0));

				if (overlaps)
				{
					w0 /= area;
					w1 /= area;
					w2 /= area;

					float u = w0 * v0.uv.u + w1 * v1.uv.u + w2 * v2.uv.u;
					float v = w0 * v0.uv.v + w1 * v1.uv.v + w2 * v2.uv.v;
					auto color = 0xFFFFFFFF;
					if (m_Texture) color = m_Texture->GetTexel(u, v);
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

	void Index::Initialize()
	{
		auto& window = Application::Instance().GetWindowHandle();
		auto rect = Vector4(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
		m_Rasterizer.SetTexture(std::make_unique<Texture>("Assets/Textures/container.png"));

		Vertex vertices[]
		{
			Vector2(-0.5f, -0.5f), Vector2(0.0f, 1.0f),	// Bottom left
			Vector2(-0.5f,  0.5f), Vector2(0.0f, 0.0f),	// Top left
			Vector2( 0.5f, -0.5f), Vector2(1.0f, 1.0f),	// Bottom right
			Vector2( 0.5f,  0.5f), Vector2(1.0f, 0.0f),	// Top right
		};
		auto verticesCount = (uint32_t)(sizeof(vertices) / sizeof(Vertex));
		for (uint32_t i = 0; i < verticesCount; ++i) m_Vertices.push_back(vertices[i]);

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 1, 3
		};
		auto indicesCount = (uint32_t)(sizeof(indices) / sizeof(uint32_t));
		for (uint32_t i = 0; i < indicesCount; ++i) m_Indices.push_back(indices[i]);
	}

	void Index::Render()
	{
		RendererContext::SetClearColor(m_ClearColor);

		for (int i = 0; i < (int)(m_Indices.size() / 3); ++i)
		{
			Vertex v0 = m_Vertices[m_Indices[i * 3 + 0]];
			Vertex v1 = m_Vertices[m_Indices[i * 3 + 1]];
			Vertex v2 = m_Vertices[m_Indices[i * 3 + 2]];
			m_Rasterizer.DrawTriangle(v0, v1, v2);
		}
	}

	void Index::GUI()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::ColorEdit4("Clear Color", m_ClearColor.values);
		ImGui::End();
	}

	void Index::OnWindowResize(const FW::Event::WindowResizeEvent& event)
	{
		auto rect = Vector4(0, 0, (float)event.GetWidth(), (float)event.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}
}