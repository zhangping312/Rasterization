#pragma once

#include <Logger/Logger.h>

template<typename T>
uint32_t FormatColor(T r, T g, T b, T a)
{
	LOG_ASSERT(false, "Unknow type!");
	return 0;
}

template<>
inline uint32_t FormatColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	auto color = (uint32_t) a << 24 | (uint32_t) b << 16 | (uint32_t) g << 8 | (uint32_t)r;
	return color;
}

template<>
inline uint32_t FormatColor(float r, float g, float b, float a)
{
	auto color = FormatColor((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(a * 255));
	return color;
}