#pragma once

#include "Graphics/Texture.h"

#include <Level/ILevel.h>
#include <Math/Vectors.h>
#include <vector>

namespace Example { namespace Example04 {} }

namespace Example::Example04
{
	struct Vertex
	{
		FW::Math::Vector2 position;
		FW::Math::Vector2 uv;
	};

	class Rasterizer
	{
	private:
		FW::Math::Vector4 m_Viewport;
		FW::Math::Vector4 m_Scissor;

		std::unique_ptr<Texture> m_Texture;

	public:
		void SetViewport(const FW::Math::Vector4& viewport) { m_Viewport = viewport; }
		void SetScissor(const FW::Math::Vector4& scissor) { m_Scissor = scissor; }

		void SetTexture(std::unique_ptr<Texture> texture) { m_Texture = std::move(texture); }
		void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	private:
		FW::Math::Vector2 ToRaster(const FW::Math::Vector2& p_ndc);
		bool IsInScissor(float w_raster, float h_raster);

		float EdgeFunction(const FW::Math::Vector2& v0, const FW::Math::Vector2 &v1, const FW::Math::Vector2 &p);
	};

	class Index : public FW::Level::ILevel
	{
	private:
		FW::Math::Vector4 m_ClearColor;

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		Rasterizer m_Rasterizer;

	public:
		virtual void Initialize() override;

		virtual void Render() override;
		virtual void GUI() override;

		virtual void OnWindowResize(const FW::Event::WindowResizeEvent& event) override;

		LEVEL_CLASS_TYPE(Example::Example04::Index)
	};
}