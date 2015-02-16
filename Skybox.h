#pragma once
#include "Object.h"
#include "Cube.h"

class Skybox :
	public Object
{
public:
	void Initialize();
	void Render();
	void Update() {};
	void HandleEvents() {};
	Skybox();
	~Skybox();
protected:
private:
	GLuint texture[5];
	GLuint cubeMapTexture;

	Cube frame;

};

