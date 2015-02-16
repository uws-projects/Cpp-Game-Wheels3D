#pragma once
#include <GL\glew.h>
#include <glm.hpp>
#include <SDL.h>
//#include "Object.h"
#include "Shader.h"
#include "Material.h"

class Cube 
	//public Object
{
public:
	Cube();
	~Cube() {};

	void UseModel(GLuint Model) { model = Model; };
	void UseMaterial(Material m)	{ material = m; };
	void UseIndex(unsigned int meshCount) { indexCount = meshCount; };
	void Initialize();
	void HandleEvents() {};
	void Update();
	void Render();
	glm::vec3& Position()	{ return position; };
	glm::vec3& Scale()		{ return scale; };
	GLuint& Model()			{ return model; };
	unsigned int &Index()	{ return indexCount; };

protected:
private:
	GLuint model;
	GLuint texture;
	Material material;
	unsigned int indexCount;
	glm::vec3 position;
	glm::vec3 scale;
};

