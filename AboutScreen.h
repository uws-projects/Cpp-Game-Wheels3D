#pragma once
#include "AbstractState.h"
#include "Sound.h"

class AboutScreen :
	public AbstractState
{
public:
	AboutScreen() {}
	~AboutScreen() {}

	void Update();
	void Render();
	void HandleEvents();

	bool OnEnter();
	bool OnExit() { SOUND->PauseMusic(); SOUND->Music(MUSICMENU, VALUES->settings[Volume_Music]); return true; }

private:

	GLuint texture;
	GLuint model;
	Light light;
	Material material;
	int indexCount;
};

