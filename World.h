#pragma once
#include "Object.h"
#include "Camera.h"

class World :
	public Object
{
public:
	World();
	World(Light l) : up(l) {}

	void Initialize();
	void Render();
	void Update() {} ;
	void HandleEvents() {};
	void UseLight(Light l) { up = l; }
	void UseMaterial(Material m) { material = m; }
	void setCamera(Camera* that) { forShader = that; }

protected:
private:
	Light left, right, up, down;
	Material material;
	std::vector <GLuint> textures;
	std::vector <GLuint> normalMap;
	std::vector <GLuint> models;
	std::vector <int> indexCount;
	glm::mat3 normalMatrix;
	Camera* forShader;
};

