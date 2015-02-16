#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ctime>

class Light
{
	
public:
	Light() {
		position	= glm::vec4(-125.0f, 500.0f, 250.0f, 1.0f);
		La	= glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
		Ld	= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Ls	= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		constant = 0.1f;
		linear = 0.0f;
		quadratic = 0.0f;
	}
	Light(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, 
		float constant = 1.0f, float linear = 0.0f, float quadratic = 0.0f) :
		position(position),
		La(ambient),
		Ld(diffuse),
		Ls(specular),
		constant(constant),
		linear(linear),
		quadratic(quadratic)
	{}

	glm::vec4 position;
	glm::vec4 La;			
	glm::vec4 Ld;			
	glm::vec4 Ls;
	float constant;
	float linear;
	float quadratic;

};

