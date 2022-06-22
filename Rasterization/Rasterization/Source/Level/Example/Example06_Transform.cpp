#include "Level/Example/Example06_Transform.h"
#include "Graphics/RendererContext.h"
#include "Application.h"

#include <Math/Math.h>
#include <Math/Matrices.h>
#include <Logger/Logger.h>
#include <imgui.h>

using namespace FW::Math;

namespace Example::Example06
{
	void Rasterizer::DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		m_Vertex[0].rhw = 1.0f / v0.position.z;
		m_Vertex[1].rhw = 1.0f / v1.position.z;
		m_Vertex[2].rhw = 1.0f / v2.position.z;
		m_Vertex[0].position =  Vector4(v0.position.x, v0.position.y, v0.position.z, 1.0f);
		m_Vertex[1].position =  Vector4(v1.position.x, v1.position.y, v1.position.z, 1.0f);
		m_Vertex[2].position =  Vector4(v2.position.x, v2.position.y, v2.position.z, 1.0f);
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

	void Transform::Initialize()
	{
		auto& window = Application::Instance().GetWindowHandle();
		auto rect = Vector4(0, 0, (float)window.GetWidth(), (float)window.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);

		Vertex vertices[] =
		{
			Vector3(-0.5f, -0.5f, -0.5f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector3(-0.5f, +0.5f, -0.5f), Vector4(0.0f, 0.0f, 0.0f, 0.0f),
			Vector3(+0.5f, +0.5f, -0.5f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),
			Vector3(+0.5f, -0.5f, -0.5f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
			Vector3(-0.5f, -0.5f, +0.5f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
			Vector3(-0.5f, +0.5f, +0.5f), Vector4(1.0f, 1.0f, 0.0f, 1.0f),
			Vector3(+0.5f, +0.5f, +0.5f), Vector4(0.0f, 1.0f, 1.0f, 1.0f),
			Vector3(+0.5f, -0.5f, +0.5f), Vector4(1.0f, 0.0f, 1.0f, 1.0f)
		};
		auto verticesCount = (uint32_t)(sizeof(vertices) / sizeof(Vertex));
		for (uint32_t i = 0; i < verticesCount; ++i) m_Vertices.push_back(vertices[i]);

		uint32_t indices[] =
		{
			// front face
			0, 1, 2,
			0, 2, 3,
			// back face
			4, 6, 5,
			4, 7, 6,
			// left face
			4, 5, 1,
			4, 1, 0,
			// right face
			3, 2, 6,
			3, 6, 7,
			// top face
			1, 5, 6,
			1, 6, 2,
			// bottom face
			4, 0, 3,
			4, 3, 7
		};
		auto indicesCount = (uint32_t)(sizeof(indices) / sizeof(uint32_t));
		for (uint32_t i = 0; i < indicesCount; ++i) m_Indices.push_back(indices[i]);
	}

	void Transform::Render()
	{
		RendererContext::SetClearColor(m_ClearColor);

		Matrix4 M, S, R, T;
		S = Matrix4::Scale(m_Scale);
		Vector3 axis = Vector3::Normalize(Vector3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
		R = Matrix4::RotateArbitraryAxis(axis, ToRadian(m_Rotation.w));
		T = Matrix4::Translate(m_Position);
		M = S * R * T;
		for (int i = 0; i < (int)(m_Indices.size() / 3); ++i)
		{
			Vertex v0 = m_Vertices[m_Indices[i * 3 + 0]];
			Vertex v1 = m_Vertices[m_Indices[i * 3 + 1]];
			Vertex v2 = m_Vertices[m_Indices[i * 3 + 2]];
			v0.position = Matrix4::Transform(v0.position, M);
			v1.position = Matrix4::Transform(v1.position, M);
			v2.position = Matrix4::Transform(v2.position, M);
			m_Rasterizer.DrawTriangle(v0, v1, v2);
		}
	}

	void Transform::GUI()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::ColorEdit4("Clear Color", m_ClearColor.values);
		ImGui::DragFloat3("Scale", m_Scale.values, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat4("Rotation", m_Rotation.values);
		ImGui::DragFloat3("Position", m_Position.values, 0.01f, -1.0f, 1.0f);
		ImGui::End();
	}

	void Transform::OnWindowResize(const FW::Event::WindowResizeEvent& event)
	{
		auto rect = Vector4(0, 0, (float)event.GetWidth(), (float)event.GetHeight());
		m_Rasterizer.SetViewport(rect);
		m_Rasterizer.SetScissor(rect);
	}
}