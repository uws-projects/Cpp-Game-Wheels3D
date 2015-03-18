#pragma once
#include "Object.h"

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
	void SetSpeed(float *Speed) { speed = Speed; }
	void SetTimer(double *Time) { timer = Time; }
	void SetGear(int *Gear)		{ gear = Gear; }

	std::string text;
	GLuint plane;
	int indexCount;
	Light light;
	Material material;
	float* speed;
	double* timer;
	int* gear;
	GLuint SpeedTexture;
	GLuint TimerTexture;
	GLuint GearTexture;
	GLuint BackgroundTexture;
};

