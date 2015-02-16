#include "Cube.h"
#include "Load.h"


Cube::Cube()
{
}

void Cube::Initialize()
{
	model = Load::Obj("model/cubeModel.obj");
	indexCount = Load::meshCount();
	material.Ka = glm::vec4(0.0, 0.0, 0.0, 1.0);
	material.Kd = glm::vec4(0.85, 0.85, 0.0, 1.0);
	material.Ks = glm::vec4(0.3, 0.3, 0.3, 1.0);
	material.shininess = 1.0f;

	texture = Load::BMP("textures/fabric.bmp");
	scale = glm::vec3(0.1f);
	position = glm::vec3(0.0f);
}

void Cube::Render()
{
	Shader::Push();
	{
		Shader::Top() = glm::translate(Shader::Top(), position);
		Shader::Top() = glm::scale(Shader::Top(), scale);
		Shader::AddMaterial(material);
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		Shader::SetUniform("MVP", Shader::ProjectionMatrix()*Shader::Top());
		glBindVertexArray(model);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	Shader::Pop();
}

void Cube::Update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_KP_4]) position.x -= 0.1f;
	if (keys[SDL_SCANCODE_KP_6]) position.x += 0.1f;
	if (keys[SDL_SCANCODE_KP_8]) position.z -= 0.1f;
	if (keys[SDL_SCANCODE_KP_2]) position.z += 0.1f;
	if (keys[SDL_SCANCODE_KP_1]) position.y -= 0.1f;
	if (keys[SDL_SCANCODE_KP_7]) position.y += 0.1f;
};

