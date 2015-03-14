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

	std::string text;
	GLuint plane;
	int indexCount;
	Light light;
	Material material;
	float* speed;
	double* timer;
	GLuint SpeedTexture;
	GLuint TimerTexture;

};

