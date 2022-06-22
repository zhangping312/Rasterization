#include "Level/Example/Example09_PerspectiveCorrect.h"
#include "Graphics/RendererContext.h"
#include "Application.h"

#include <Math/Math.h>
#include <Math/Matrices.h>
#include <Logger/Logger.h>
#include <imgui.h>

using namespace FW::Math;

namespace Example::Example09
{
	void Rasterizer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, bool perspectiveCorrect)
	{
		m_Vertex[0].rhw = 1.0f / v0.position.w;
		m_Vertex[1].rhw = 1.0f / v1.position.w;
		m_Vertex[2].rhw = 1.0f / v2.position.w;
		m_Vertex[0].position =  v0.position * m_Vertex[0].rhw;
		m_Vertex[1].position =  v1.position * m_Vertex[1].rhw;
		m_Vertex[2].position =  v2.position * m_Vertex[2].rhw;
		m_Vertex[0].screenPosition = ToRaster(m_Vertex[0].position);
		m_Vertex[1].screenPosition = ToRaster(m_Vertex[1].position);
		m_Vertex[2].screenPosition = ToRaster(m_Vertex[2].position);

		// Check if triangle is backfacing.
		float area = EdgeFunction(m_Vertex[0].screenPosition, m_Vertex[1].screenPosition, m_Vertex[2].screenPosition);
        if (area >= 0) return;

		Vector2 min, max;
		min.x = std::max(m_Viewport.x, std::min(m_Vertex[0].screenPosition.x, std::min(m_Vertex[1].screenPosition.x, m_Vertex[2].screenPosition.x)));
		min.y = std::max(m_Viewport.y, std::min(m_Vertex[0].screenPosition.y, std::min(m_Vertex[1].screenPosition.y, m_Vertex[2].screenPosition.y)));
		max.x = std::min(m_Viewport.z - 1.0f, std::max(m_Vertex[0].screenPosition.x, std::max(m_Vertex[1].screenPosition.x, m_Vertex[2].screenPosition.x)));
		max.y = std::min(m_Viewport.w - 1.0f, std::max(m_Vertex[0].screenPosition.y, std::max(m_Vertex[1].screenPosition.y, m_Vertex[2].screenPosition.y)));

		// Add 0.5 to sample at pixel centers.
        for (float x = min.x + 0.5f, xm = max.x + 0.5f; x <= xm; x += 1.0f)
		{
			for (float y = min.y + 0.5f, ym = max.y + 0.5f; y <= ym; y += 1.0f)
			{
				if (!IsInScissor(x, y)) continue;

				Vector2 p(x, y);

				float w0 = EdgeFunction(m_Vertex[1].screenPosition, m_Vertex[2].screenPosition, p);
				float w1 = EdgeFunction(m_Vertex[2].screenPosition, m_Vertex[0].screenPosition, p);
				float w2 = EdgeFunction(m_Vertex[0].screenPosition, m_Vertex[1].screenPosition, p);

				Vector2 edge0 = m_Vertex[2].screenPosition - m_Vertex[1].screenPosition;
				Vector2 edge1 = m_Vertex[0].screenPosition - m_Vertex[2].screenPosition;
				Vector2 edge2 = m_Vertex[1].screenPosition - m_Vertex[0].screenPosition;

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
					float rhw = w0 * m_Vertex[0].rhw + w1 * m_Vertex[1].rhw + w2 * m_Vertex[2].rhw;
					float z = 1.0f / rhw;
					if (z < RendererContext::GetDepth((int)x, (int)y))
					{
						RendererContext::SetDepth((int)x, (int)y, z);
						float u, v;
						if (perspectiveCorrect)
						{
							u = (w0 * m_Vertex[0].rhw * v0.uv.u + w1 * m_Vertex[1].rhw * v1.uv.u + w2 * m_Vertex[2].rhw * v2.uv.u) * z;
							v = (w0 * m_Vertex[0].rhw * v0.uv.v + w1 * m_Vertex[1].rhw * v1.uv.v + w2 * m_Vertex[2].rhw * v2.uv.v) * z;
						}
						else
						{
							u = w0 * v0.uv.u + w1 * v1.uv.u + w2 * v2.uv.u;
							v = w0 * v0.uv.v + w1 * v1.uv.v + w2 * v2.uv.v;
						}
						auto color = 0xFFFFFFFF;
						if (m_Texture) color = m_Texture->GetTexel(u, v);
						RendererContext::DrawPixel((int)x, (int)y, color);
					}
				}
			}
		}
	}

	Vector2 Rasterizer::ToRaster(const Vector4& p_ndc)
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

	void PerspectiveCorrect::Initialize()
	{
		auto& window = Application::Instance().GetWindowHandle();
		auto rect = Vector4(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
		m_Rasterizer.SetTexture(std::make_unique<Texture>("Assets/Textures/container.png"));

		Vertex vertices[] =
		{
			Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f),	// Bottom left
			Vector4(-0.5f,  0.5f, 0.0f, 1.0f), Vector2(0.0f, 0.0f),	// Top left
			Vector4( 0.5f, -0.5f, 0.0f, 1.0f), Vector2(1.0f, 1.0f),	// Bottom right
			Vector4( 0.5f,  0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f),	// Top right
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

	void PerspectiveCorrect::Render()
	{
		RendererContext::SetClearColor(m_ClearColor);

		Matrix4 MVP, S, R, T;
		S = Matrix4::Scale(m_Scale);
		Vector3 axis = Vector3::Normalize(Vector3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
		R = Matrix4::RotateArbitraryAxis(axis, ToRadian(m_Rotation.w));
		T = Matrix4::Translate(m_Position);
		Matrix4 V = Matrix4::LookAt(m_CameraPosition, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		auto& window = Application::Instance().GetWindowHandle();
		Matrix4 P = Matrix4::PerspectiveFov(ToRadian(45.0f), window.GetAspectRatio(), 1.0f, 1000.0f);
		MVP = S * R * T * V * P;
		for (int i = 0; i < (int)(m_Indices.size() / 3); ++i)
		{
			Vertex v0 = m_Vertices[m_Indices[i * 3 + 0]];
			Vertex v1 = m_Vertices[m_Indices[i * 3 + 1]];
			Vertex v2 = m_Vertices[m_Indices[i * 3 + 2]];
			v0.position = Matrix4::Transform(v0.position, MVP);
			v1.position = Matrix4::Transform(v1.position, MVP);
			v2.position = Matrix4::Transform(v2.position, MVP);
			m_Rasterizer.DrawTriangle(v0, v1, v2, m_PerspectiveCorrect);
		}
	}

	void PerspectiveCorrect::GUI()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::ColorEdit4("Clear Color", m_ClearColor.values);
		if (ImGui::CollapsingHeader("Cube"))
		{
			ImGui::DragFloat3("Scale##Cube", m_Scale.values);
			ImGui::DragFloat4("Rotation##Cube", m_Rotation.values);
			ImGui::DragFloat3("Position##Cube", m_Position.values);
		}
		if (ImGui::CollapsingHeader("Camera"))
			ImGui::DragFloat3("Position##Camera", m_CameraPosition.values, 0.1f);
		ImGui::Checkbox("Is Perspective Correct", &m_PerspectiveCorrect);
		ImGui::End();
	}

	void PerspectiveCorrect::OnWindowResize(const FW::Event::WindowResizeEvent& event)
	{
		auto rect = Vector4(0, 0, (float)event.GetWidth(), (float)event.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}
}