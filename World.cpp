#include "World.h"

World::World()
{
}

const int NUMBEROFOBJECTS = 17;

void World::Initialize()
{
	char buffer[128];
	// loading all textures and objects
	// textures and models should be named consecutively
	// and follow a certain format textureXX (where XX is the 
	// last texture value + 1) and same for modelXX
	for (int i = 0; i != NUMBEROFOBJECTS; i++)
	{
		std::string temp = _itoa(i, buffer, 10);
		std::string path = "textures/texture" + temp + ".bmp";
		textures.push_back(Load::BMP((char*)path.c_str()));
		path = "normals/normal" + temp + ".bmp";
		normalMap.push_back(Load::BMP((char*)path.c_str()));
		path = "model/model" + temp + ".obj";
		models.push_back(Load::Obj((char*)path.c_str()));
		indexCount.push_back(Load::meshCount());
	}

	left.position = glm::vec4(-105.0f, 1000.0f, -75.0f, 1.0f);
	right.position = glm::vec4(295.0f, 1000.0f, 325.0f, 1.0f);
	up.position = glm::vec4(-105.0f, 1000.0f, 325.0f, 1.0f);
	left.position = glm::vec4(295.0f, 1000.0f, -75.0f, 1.0f);

	std::cin;
}

bool enabled = true;
float offset = 0.00f;
float scale = 0.02f;

void World::Render()
{
	
	Shader::Push();
	{
		Shader::Use(Shader::DiffusePerVertex());
		Shader::AddLight(left, "light[0]");
		Shader::AddLight(right, "light[1]");
		Shader::AddLight(up, "light[2]");
		Shader::AddLight(down, "light[3]");
		Shader::AddMaterial(material);

		Shader::SetUniform("ModelViewMatrix",Shader::ModelViewMatrix());
		Shader::SetUniform("ProjectionMatrix", Shader::ProjectionMatrix());
		Shader::SetUniform("NormalMatrix", Shader::NormalMatrix());
		Shader::SetUniform("MVP", Shader::ProjectionMatrix()*Shader::Top());
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	
		for (int i = 0; i != NUMBEROFOBJECTS; i++)

		{
			
			// atach texture to all the loaded models
			Shader::Bind(0, "colourMap", textures[i]);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			Shader::Bind(1, "normalMap", normalMap[i]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap[i]);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			// draw
			glBindVertexArray(models[i]);
			glDrawElements(GL_TRIANGLES, indexCount[i], GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	Shader::Pop();
}