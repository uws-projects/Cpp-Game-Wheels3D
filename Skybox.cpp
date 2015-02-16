#include "Skybox.h"


Skybox::Skybox()
{
}

void Skybox::Initialize()
{
	const char *files[6] = {
		"textures/skybox/texture1.bmp", 
		"textures/skybox/texture0.bmp", 
		"textures/skybox/texture2.bmp", 
		"textures/skybox/texture3.bmp", 
		"textures/skybox/texture4.bmp", 
		"textures/skybox/texture5.bmp"
	};
	Load::CubeMap(files, &texture[0]);
	frame.Initialize();
}

void Skybox::Render()
{
	Shader::Use(Shader::Skybox());
	Shader::SetUniform("ProjectionMatrix", Shader::ProjectionMatrix());
	Shader::SetUniform("ModelViewMatrix", Shader::ModelViewMatrix());
	glDepthMask(GL_FALSE);
	{
		Shader::Push(glm::mat4(glm::mat3(Shader::Top())));
		{
			glCullFace(GL_FRONT);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
			Shader::Bind(0, "cubeMap", texture[0]);
			frame.Render();
			//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glCullFace(GL_BACK);
		}
		Shader::Pop();
	}
	glDepthMask(GL_TRUE);
}

Skybox::~Skybox()
{
}
