#pragma once

class LevelEditor
{
public:
	LevelEditor();
	LevelEditor(const LevelEditor& rhs) = delete;
	LevelEditor& operator=(const LevelEditor& rhs) = delete;
	~LevelEditor() = default;

	void GUI();

private:
	void ShowLevelMenu();
	void ShowFPS();
};