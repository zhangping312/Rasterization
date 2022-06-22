#include "Editor/LevelEditor.h"
#include "Application.h"

#include "Level/Example/Example01_Line.h"
#include "Level/Example/Example02_Triangle.h"
#include "Level/Example/Example03_Interpolation.h"
#include "Level/Example/Example04_Index.h"
#include "Level/Example/Example05_DepthBuffer.h"
#include "Level/Example/Example06_Transform.h"
#include "Level/Example/Example07_Camera.h"
#include "Level/Example/Example08_Project.h"
#include "Level/Example/Example09_PerspectiveCorrect.h"

#include <Level/LevelManager.h>
#include <Time/Time.h>
#include <imgui.h>

using namespace FW::Level;

#define RED_COLOR		{ 1.0f, 0.0f, 0.0f, 1.0f }
#define WHILTE_COLOR	{ 1.0f, 1.0f, 1.0f, 1.0f }

LevelEditor::LevelEditor()
{
	LevelManager::Instane().RegisterLevel<Example::Example01::Line>();
	LevelManager::Instane().RegisterLevel<Example::Example02::Triangle>();
	LevelManager::Instane().RegisterLevel<Example::Example03::Interpolation>();
	LevelManager::Instane().RegisterLevel<Example::Example04::Index>();
	LevelManager::Instane().RegisterLevel<Example::Example05::DepthBuffer>();
	LevelManager::Instane().RegisterLevel<Example::Example06::Transform>();
	LevelManager::Instane().RegisterLevel<Example::Example07::Camera>();
	LevelManager::Instane().RegisterLevel<Example::Example08::Project>();
	LevelManager::Instane().RegisterLevel<Example::Example09::PerspectiveCorrect>();

	auto& registerLevels = LevelManager::Instane().GetRegisterLevels();
	auto& lastLevelName = (--registerLevels.end())->first;
	LevelManager::Instane().LoadLevel(lastLevelName);
}

void LevelEditor::GUI()
{
	ShowLevelMenu();
	ShowFPS();
}

void LevelEditor::ShowLevelMenu()
{
	ImGui::Begin("Select Level");
	auto& registerLevels = LevelManager::Instane().GetRegisterLevels();
	for (auto iter = registerLevels.begin(); iter != registerLevels.end(); ++iter)
	{
		auto& levelName = iter->first;
		if (LevelManager::Instane().IsActive(levelName))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, RED_COLOR);
			if (ImGui::Button("Unload")) LevelManager::Instane().UnloadLevel(levelName);
			ImGui::PopStyleColor(1);
		}
		else
		{
			if (ImGui::Button(levelName.c_str()))
			{
				// Unload active levels
				LevelManager::Instane().UnloadActiveLevels();
				// Load new level
				LevelManager::Instane().LoadLevel(levelName);
			}
		}
	}
	ImGui::End();
}

void LevelEditor::ShowFPS()
{
	// Calculate FPS
	static int frameCount = 0;
	static float timeElapsed = 0.0f;
	static float fps = -1.0f;
	static float mspf;
	++frameCount;
	timeElapsed += FW::Time::GetDeltaTime();
	if (timeElapsed >= 1.0f)
	{
		fps = static_cast<float>(frameCount) / timeElapsed;
		mspf = 1000.0f / fps;
		timeElapsed -= 1.0f;
		frameCount = 0;
	}
	auto position = Application::Instance().GetWindowHandle().GetPosition();
	ImGui::SetNextWindowPos({ 0.0f, 0.0f });
	ImGui::SetNextWindowSizeConstraints({ 200.0f, 30.0f }, { 200.0f, 30.0f });
	// Set up the Stats window that have no title and background
	int flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;
	ImGui::Begin("Stats", nullptr, flags);
	ImGui::Text("FPS: %0.2f, mspf: %0.2f", fps, mspf);
	ImGui::End();
}