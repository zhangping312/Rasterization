#pragma once

#include <string>
#include <stdint.h>

enum class TEXTURE_WRAP_MODE
{
	CLAMP,
	WRAP,
	REPEAT,
};

class Texture
{
private:
	std::string m_FilePath;
	int m_Width, m_Height, m_Channel;
	uint32_t* m_Texels = nullptr;

public:
	Texture(const char* filePath);
	Texture(const Texture& rhs) = delete;
	Texture& operator=(const Texture& rhs) = delete;
	~Texture();

	uint32_t GetTexel(float u, float v) const;
};