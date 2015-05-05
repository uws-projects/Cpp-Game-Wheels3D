#pragma once
#include "AbstractState.h"
#include "Wheel.h"
#include "Skybox.h"
#include "World.h"
#include "Camera.h"
#include "HUD.h"
#include "Powerup.h"
#include "Cube.h"

struct music {
	int length;
	bool playing;

	music() { length = 0; playing = false; }
//	music(int l, bool p) { length: l; playing: p; }
};

class Play :
	public AbstractState
{
public:
	Play();
	~Play() {
		delete player; delete camera; delete skybox; delete world; delete hud;
	}

	void Update();
	void Render();
	void HandleEvents();

	bool OnEnter();
	bool OnExit();

	void Clean();

private:

	Wheel *player;
	Camera *camera;
	Skybox *skybox;
	World *world;
	HUD *hud;
	int musicTimer;
	
	music MusicSetup, MusicCredits, MusicTitle, MusicScoreboard;
	std::vector <Object*> m_object;
};

