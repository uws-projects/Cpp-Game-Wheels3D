#pragma once
#include "AbstractState.h"
#include "Sound.h"
#include "Highscore.h"

class HighScoreScreen:
	public AbstractState
{
public:
	HighScoreScreen() {}
	~HighScoreScreen() {}

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
	GLuint playerTextures[10];

	GLuint oTexture;
	GLuint scoreTextures[10];
};

