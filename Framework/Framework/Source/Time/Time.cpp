#include <Time/Time.h>

namespace FW
{
	float Time::s_DeltaTime;
	float Time::s_TotalTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Time::s_Last;

	void Time::Initialize()
	{
		s_Last = std::chrono::high_resolution_clock::now();
		s_DeltaTime = 0.0f;
		s_TotalTime = 0.0f;
	}

	void Time::Update()
	{
		auto current = std::chrono::high_resolution_clock::now();
		s_DeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(current - s_Last).count() * 0.001f * 0.001f * 0.001f;
		if (s_DeltaTime < 0.0f) s_DeltaTime = 0.0f;
		s_TotalTime += s_DeltaTime;
		s_Last = current;
	}

	float Time::GetDeltaTime()
	{
		return s_DeltaTime;
	}

	float Time::GetTotalTime()
	{
		return s_TotalTime;
	}
}