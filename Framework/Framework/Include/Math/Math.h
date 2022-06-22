#pragma once

namespace FW::Math
{
	constexpr float PI = 3.141592653589793f;
	constexpr float TwoPI = PI * 2.0f;
	constexpr float PIOver2 = PI / 2.0f;

	static inline float Fabs(float value)
	{
		return value < 0.0f ? -value : value;
	}

	static inline float ToRadian(float angle)
	{
		return angle * PI / 180.0f;
	}

	static inline float ToAngle(float radian)
	{
		return radian * 180.0f / PI ;
	}

	template<typename T>
	static inline T Clamp(const T& value, const T& min, const T& max)
	{
		if (value < min) 		return min;
		else if (value > max) 	return max;
		else 					return value;
	}
};