#pragma once
#include "AbstractState.h"
class Logo :
	public AbstractState
{
public:
	Logo() {}
	~Logo() {}

	void Update();
	void Render();
	void HandleEvents();

	bool OnEnter();
	bool OnExit() { return true; }

private:
	int initialTime, totalTime; 
	float transparency;

	GLuint texture;
	GLuint model;

};

