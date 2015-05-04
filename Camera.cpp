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

void Camera::HandleEvents()
{
}

void Camera::Render()
{
	Shader::Push(view);
}