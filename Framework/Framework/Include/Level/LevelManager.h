#pragma once

#include <Level/ILevel.h>

#include <vector>
#include <functional>
#include <unordered_map>

namespace FW::Level
{
	using CreateLevelFunc = std::function<ILevel*()>;

	class LevelManager
	{
	private:
		static LevelManager* s_Instance;
		std::vector<std::pair<std::string, CreateLevelFunc>> m_RegisterLevels;
		std::unordered_map<std::string, std::unique_ptr<ILevel>> m_ActiveLevels;

	private:
		LevelManager() = default;
	public:
		LevelManager(const LevelManager& rhs) = delete;
		LevelManager& operator=(const LevelManager& rhs) = delete;
		~LevelManager();

		template<typename T>
		void RegisterLevel();

		ILevel& LoadLevel(const std::string& levelName);
		void UnloadLevel(const std::string& levelName);
		void UnloadLevel(ILevel* level);
		void UnloadActiveLevels();

		static LevelManager& Instane();
		const std::vector<std::pair<std::string, CreateLevelFunc>>& GetRegisterLevels() { return m_RegisterLevels; }
		const std::unordered_map<std::string, std::unique_ptr<ILevel>>& GetActiveLevels() { return m_ActiveLevels; }
		bool IsActive(const std::string& levelName);
	};
}

#include <Level/LevelManager.inl>