#pragma once

namespace FW::Level
{
	template<typename T>
	void LevelManager::RegisterLevel()
	{
		auto levelName = T::GetLevelName();
		bool isFound = false;
		for (auto iter : m_RegisterLevels)
		{
			if (0 == levelName.compare(iter.first))
			{
				isFound = true;
				break;
			}
		}
		if (isFound) return;

		auto func = []() -> ILevel* { return new T(); };
		m_RegisterLevels.emplace_back(std::make_pair(levelName, func));
	}
}