#pragma once
#include "Object.h"
#include "Wheel.h"

class HUD :
	public Object
{
public:
	HUD() {}
	~HUD();

	void Initialize();
	void Update();
	void Render();
	void HandleEvents() {}
	void AttachPlayer(Wheel* user) { player = user; }
	
	std::string text;
	GLuint plane;
	int indexCount;
	Light light;
	Material material;
	float* speed;
	double* timer;
	double lapTime;
	int* gear;
	GLuint SpeedTexture;
	GLuint TimerTexture;
	GLuint GearTexture;
	GLuint BackgroundTexture;
	GLuint StopLight[4];

	bool showStopLight;
	int stopLightDelay;
	int lamp; 
	Wheel* player;
};

