#include <Level/LevelManager.h>
#include <Logger/Logger.h>

namespace FW::Level
{
	LevelManager* LevelManager::s_Instance = nullptr;

	LevelManager::~LevelManager()
	{
		for (auto& iter : m_ActiveLevels) iter.second->Release();
	}

	ILevel& LevelManager::LoadLevel(const std::string& levelName)
	{
		if (m_ActiveLevels.find(levelName) == m_ActiveLevels.end())
		{
			for (auto iter : m_RegisterLevels)
			{
				if (0 == levelName.compare(iter.first))
				{
					m_ActiveLevels.emplace(levelName, iter.second());
					m_ActiveLevels[levelName]->Initialize();
					break;
				}
			}
		}
		LOG_ASSERT(m_ActiveLevels.find(levelName) != m_ActiveLevels.end(), "Failed to load scene!");
		return *m_ActiveLevels[levelName].get();
	}

	void LevelManager::UnloadLevel(const std::string& levelName)
	{
		if (m_ActiveLevels.size() > 0 && m_ActiveLevels.find(levelName) != m_ActiveLevels.end())
		{
			m_ActiveLevels[levelName]->Release();
			m_ActiveLevels.erase(levelName);
		}
	}

	void LevelManager::UnloadLevel(ILevel* level)
	{
		UnloadLevel(level->GetName());
	}

	void LevelManager::UnloadActiveLevels()
	{
		for (auto iter = m_ActiveLevels.cbegin(); iter != m_ActiveLevels.cend();)
		{
			iter->second->Release();
			m_ActiveLevels.erase(iter++);
		}
	}

	LevelManager& LevelManager::Instane()
	{
		if (!s_Instance) s_Instance = new LevelManager();
		return *s_Instance;
	}

	bool LevelManager::IsActive(const std::string& levelName)
	{
		return m_ActiveLevels.find(levelName) != m_ActiveLevels.end();
	}
}