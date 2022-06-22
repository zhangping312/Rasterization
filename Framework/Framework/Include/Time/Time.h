#pragma once

#include <chrono>

namespace FW
{
	class Time
	{
	private:
		static float s_DeltaTime;
		static float s_TotalTime;
		static std::chrono::time_point<std::chrono::high_resolution_clock> s_Last;

	public:
		static void Initialize();
		static void Update();

		static float GetDeltaTime();
		static float GetTotalTime();
	};
}