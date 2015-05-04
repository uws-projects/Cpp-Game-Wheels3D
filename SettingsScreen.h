#pragma once
#include "AbstractState.h"
#include "Sound.h"

class SettingsScreen :
	public AbstractState
{
public:
	SettingsScreen() {}
	~SettingsScreen() {}

	void Update();
	void Render();
	void HandleEvents();

	bool OnEnter();
	bool OnExit() { SOUND->PauseMusic(); SOUND->Music(MUSICMENU, MusicVolume); return true; }

private:

	GLuint texture;
	GLuint model;
	Light light;
	Material material;
	int indexCount;
	std::string name;
	void displayString(float x, float y, float width, float height, std::string text, int r, int g, int b);

};

