#include "Graphics/Texture.h"
#include "Graphics/RendererUtility.h"

#include <Logger/Logger.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const char* filePath)
{
	int channel;
	auto* image = stbi_load(filePath, &m_Width, &m_Height, &channel, 0);
	if (image != nullptr)
	{
		m_Texels = new uint32_t[m_Width * m_Height];
		int index = 0;
		if (3 == channel)
		{
			for (int w = 0; w < m_Width; ++w)
			{
				for (int h = 0; h < m_Height; ++h)
				{
					uint8_t r = (uint8_t)image[index * 3];
					uint8_t g = (uint8_t)image[index * 3 + 1];
					uint8_t b = (uint8_t)image[index * 3 + 2];
					uint8_t a = 255;
					m_Texels[index] = FormatColor(r, g, b, a);
					++index;
				}
			}
		}
		else if (4 == channel)
		{
			for (int w = 0; w < m_Width; ++w)
			{
				for (int h = 0; h < m_Height; ++h)
				{
					uint8_t r = (uint8_t)image[index * 4];
					uint8_t g = (uint8_t)image[index * 4 + 1];
					uint8_t b = (uint8_t)image[index * 4 + 2];
					uint8_t a = (uint8_t)image[index * 4 + 3];
					m_Texels[index] = FormatColor(r, g, b, a);
					++index;
				}
			}
		}
		stbi_image_free(image);
	}
	else
	{
		LOG_ERROR("Failed to load the texture: {}!", filePath);
	}
}

Texture::~Texture()
{
	if (m_Texels) delete[] m_Texels;
	m_Texels = nullptr;
}

uint32_t Texture::GetTexel(float u, float v) const
{
	if (!m_Texels) return 0xFFFFFFFF;

	float real_u = u;
	float real_v = v;
	int real_x = (int)(real_u * (m_Width - 1));
	int real_y = (int)(real_v * (m_Height - 1));
	LOG_ASSERT(0 <= real_x && real_x < m_Width && 0 <= real_y && real_y < m_Height, "Invalid operation!");
	int index = real_x + (real_y) * m_Width;
	return m_Texels[index];
}