#pragma once
#include <glm.hpp>
class Material
{
public:
	Material() {
		shininess = 200.0f;
		Ka = glm::vec4(glm::vec3(0.0f), 1.0f);
		Kd = glm::vec4(glm::vec3(0.33f), 1.0f);
		Ks = glm::vec4(glm::vec3(0.33f), 1.0f);
	}

	Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess):
		Ka(ambient),
		Kd(diffuse),
		Ks(specular),
		shininess(shininess)
	{}
	float		shininess;
	glm::vec4	Ka;
	glm::vec4	Kd;
	glm::vec4	Ks;
};
