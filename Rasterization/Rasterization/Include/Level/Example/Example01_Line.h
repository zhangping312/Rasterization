#pragma once

#include <Level/ILevel.h>
#include <Math/Vectors.h>

namespace Example { namespace Example01 {} }

namespace Example::Example01
{
	class Rasterizer
	{
	private:
		FW::Math::Vector4 m_Viewport;
		FW::Math::Vector4 m_Scissor;

	public:
		void SetViewport(const FW::Math::Vector4& viewport) { m_Viewport = viewport; }
		void SetScissor(const FW::Math::Vector4& scissor) { m_Scissor = scissor; }

		void DDALine(const FW::Math::Vector2& p0, const FW::Math::Vector2& p1, const FW::Math::Vector4& color);
		void BresenhamLine(const FW::Math::Vector2& p0, const FW::Math::Vector2& p1, const FW::Math::Vector4& color);

	private:
		FW::Math::Vector2 ToRaster(const FW::Math::Vector2& p_ndc);
		bool IsInScissor(float w_raster, float h_raster);
	};

	class Line : public FW::Level::ILevel
	{
	private:
		FW::Math::Vector4 m_ClearColor;

		FW::Math::Vector2 m_LinePoint1 = { -0.5f, -0.5f };
		FW::Math::Vector2 m_LinePoint2 = {  0.5f,  0.5f };
		FW::Math::Vector4 m_LineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		bool m_IsDDALine = true;

		Rasterizer m_Rasterizer;

	public:
		virtual void Initialize() override;

		virtual void Render() override;
		virtual void GUI() override;

		virtual void OnWindowResize(const FW::Event::WindowResizeEvent& event) override;

		LEVEL_CLASS_TYPE(Example::Example01::Line)
	};
}