#pragma once

#include <Level/ILevel.h>
#include <Math/Vectors.h>
#include <vector>

namespace Example { namespace Example03 {} }

namespace Example::Example03
{
	struct Vertex
	{
		FW::Math::Vector2 position;
		FW::Math::Vector4 color;
	};

	class Rasterizer
	{
	private:
		FW::Math::Vector4 m_Viewport;
		FW::Math::Vector4 m_Scissor;

	public:
		void SetViewport(const FW::Math::Vector4& viewport) { m_Viewport = viewport; }
		void SetScissor(const FW::Math::Vector4& scissor) { m_Scissor = scissor; }

		void BarycentricTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
		void EdgeFunctionTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	private:
		FW::Math::Vector2 ToRaster(const FW::Math::Vector2& p_ndc);
		bool IsInScissor(float w_raster, float h_raster);

		float EdgeFunction(const FW::Math::Vector2& v0, const FW::Math::Vector2 &v1, const FW::Math::Vector2 &p);
		bool IsTopLeft(const FW::Math::Vector2& v0, const FW::Math::Vector2& v1);
	};

	class Interpolation : public FW::Level::ILevel
	{
	private:
		FW::Math::Vector4 m_ClearColor;

		Vertex m_Vertex0 = { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } };
		Vertex m_Vertex1 = { {  0.0f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } };
		Vertex m_Vertex2 = { {  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
		FW::Math::Vector4 m_TriangleColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::vector<std::string> m_ToggleDescriptions =
		{
			"Barycentric",
			"Edge Function"
		};
		int m_SelectToggle = 0;

		Rasterizer m_Rasterizer;

	public:
		virtual void Initialize() override;

		virtual void Render() override;
		virtual void GUI() override;

		virtual void OnWindowResize(const FW::Event::WindowResizeEvent& event) override;

		LEVEL_CLASS_TYPE(Example::Example03::Interpolation)
	};
}