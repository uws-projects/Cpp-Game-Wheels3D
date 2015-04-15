#pragma once
#include "AbstractState.h"
class Menu :
	public AbstractState
{
public:
	Menu() {}
	~Menu() {}

	void Update();
	void Render();
	void HandleEvents();

	bool OnEnter();
	bool OnExit() { return true; }

private:
	unsigned option;
	GLuint texture[7];
	GLuint model;

};
