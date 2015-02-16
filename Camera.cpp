#include "Camera.h"



Camera::Camera()
{
}

void Camera::Initialize()
{
	eye = glm::vec3(0.0f, 0.2f, 2.0f);
	at = glm::vec3(-2.0f, 0.0f, 2.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Update()
{
	SDL_ShowCursor(false);
	view = glm::lookAt(eye, at, up);
}

const float MOUSEHDEGREE = WIDTH / 180;		// value for 1 degree in pixels horizontally
const float MOUSEVDEGREE = HEIGHT / 180;	// value for 1 degree in pixels vertically

bool JoystickOff = true;

void Camera::HandleEvents()
{
}



void Camera::Render()
{
	Shader::Push(view);
}