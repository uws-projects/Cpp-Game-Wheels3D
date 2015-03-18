#pragma once
#include "AbstractState.h"
#include "Wheel.h"
#include "Skybox.h"
#include "World.h"
#include "Camera.h"
#include "HUD.h"
#include "Powerup.h"
#include "Cube.h"

class Play :
	public AbstractState
{
public:
	Play();
	~Play();

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
	HUD *speedometer;
	Powerup *powerupHappyWheel;
	Powerup *powerupTwister;

	std::vector <Object*> m_object;
};

