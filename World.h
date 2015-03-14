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
	void HandleEvents()
	{
		if (JOY_X) {
			std::cout << "X executed \n" << quadratic;
			quadratic += 0.01f; linear = quadratic;
		}
		if (JOY_A) { 
			std::cout << "A executed \n" << quadratic; 
			quadratic -= 0.01f; linear = quadratic;
		};
	}
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
	float quadratic, linear, constant;
};

