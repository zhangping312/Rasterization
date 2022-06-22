#pragma once

#include <Level/ILevel.h>
#include <Math/Vectors.h>
#include <vector>

namespace Example { namespace Example06 {} }

namespace Example::Example06
{
	struct Vertex
	{
		FW::Math::Vector3 position;
		FW::Math::Vector4 color;
	};

	class Rasterizer
	{
	private:
		FW::Math::Vector4 m_Viewport;
		FW::Math::Vector4 m_Scissor;

		struct _Vertex
		{
			FW::Math::Vector4 position;
			FW::Math::Vector2 screenPosition;
			float rhw;	// Reciprocal of the Homogeneous W
		};
		_Vertex m_Vertex[3];

	public:
		void SetViewport(const FW::Math::Vector4& viewport) { m_Viewport = viewport; }
		void SetScissor(const FW::Math::Vector4& scissor) { m_Scissor = scissor; }

		void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	private:
		FW::Math::Vector2 ToRaster(const FW::Math::Vector4& p_ndc);
		bool IsInScissor(float w_raster, float h_raster);

		float EdgeFunction(const FW::Math::Vector2& v0, const FW::Math::Vector2 &v1, const FW::Math::Vector2 &p);
	};

	class Transform : public FW::Level::ILevel
	{
	private:
		FW::Math::Vector4 m_ClearColor;

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		FW::Math::Vector3 m_Scale = { 1.0f, 1.0f, 1.0f, };
		FW::Math::Vector4 m_Rotation = { 0.0f, 0.0f, 1.0f, 0.0f };
		FW::Math::Vector3 m_Position = { 0.0f, 0.0f, 0.0f };

		Rasterizer m_Rasterizer;

	public:
		virtual void Initialize() override;


		virtual void Render() override;
		virtual void GUI() override;

		virtual void OnWindowResize(const FW::Event::WindowResizeEvent& event) override;

		LEVEL_CLASS_TYPE(Example::Example06::Transform)
	};
}