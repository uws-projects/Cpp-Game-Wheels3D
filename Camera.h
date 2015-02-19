#pragma once
#include "Object.h"

#define DEGREES 0.017453293f

class Camera: 
	public Object
{
public:
	Camera();

public:
	void Initialize();
	void Update();
	void HandleEvents();
	void Render();
	glm::vec3& Position() { return eye; }
	glm::vec3& Target()	{ return at; }
	glm::vec3& UpDirection() { return up; }
	glm::mat4& View() { return view; }

private:
	glm::vec3 eye, at, up;
	glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d, GLfloat rotation) {
		return glm::vec3(pos.x + d*std::sin(angle*DEGREES), pos.y + d*std::sin(rotation*DEGREES), pos.z - d*std::cos(angle*DEGREES));
	}
	glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
		return glm::vec3(pos.x + d*std::cos(angle*DEGREES), pos.y, pos.z + d*std::sin(angle*DEGREES));
	}
	glm::mat4 view;
};

