#include "World.h"

World::World()
{
}

const int NUMBEROFOBJECTS = 14;

void World::Initialize()
{
	models.push_back(Load::Obj("model/world/asphalt.map"));
	textures.push_back(Load::BMP("model/world/asphalt/asphalt.bmp"));
	normalMap.push_back(Load::BMP("model/world/asphalt/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/grass.map"));
	textures.push_back(Load::BMP("model/world/grass/grass.bmp"));
	normalMap.push_back(Load::BMP("model/world/grass/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/billbloard_buildings.map"));
	textures.push_back(Load::BMP("model/world/billbloard_buildings/billbloard_buildings.bmp"));
	normalMap.push_back(Load::BMP("model/world/billbloard_buildings/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/billbloard_buildings_roofs.map"));
	textures.push_back(Load::BMP("model/world/billbloard_buildings_roofs/billbloard_buildings_roofs.bmp"));
	normalMap.push_back(Load::BMP("model/world/billbloard_buildings_roofs/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/brick.map"));
	textures.push_back(Load::BMP("model/world/brick/brick.bmp"));
	normalMap.push_back(Load::BMP("model/world/brick/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	// TO DO: create normal map for connector and create textures
	models.push_back(Load::Obj("model/world/connector.map"));
	textures.push_back(Load::BMP("model/world/connector/connector.bmp"));
	normalMap.push_back(Load::BMP("model/world/connector/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/field.map"));
	textures.push_back(Load::BMP("model/world/field/field.bmp"));
	normalMap.push_back(Load::BMP("model/world/field/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/floor.map"));
	textures.push_back(Load::BMP("model/world/floor/floor.bmp"));
	normalMap.push_back(Load::BMP("model/world/floor/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/ground_floor.map"));
	textures.push_back(Load::BMP("model/world/ground_floor/ground_floor.bmp"));
	normalMap.push_back(Load::BMP("model/world/ground_floor/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/ground_and_first_floor.map"));
	textures.push_back(Load::BMP("model/world/ground_and_first_floor/ground_and_first_floor.bmp"));
	normalMap.push_back(Load::BMP("model/world/ground_and_first_floor/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/passage.map"));
	textures.push_back(Load::BMP("model/world/passage/passage.bmp"));
	normalMap.push_back(Load::BMP("model/world/passage/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/pvcwindow.map"));
	textures.push_back(Load::BMP("model/world/pvcwindow/pvcwindow.bmp"));
	normalMap.push_back(Load::BMP("model/world/pvcwindow/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/roof.map"));
	textures.push_back(Load::BMP("model/world/roof/roof.bmp"));
	normalMap.push_back(Load::BMP("model/world/roof/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	// TO DO: normal map for track
	models.push_back(Load::Obj("model/world/track.map"));
	textures.push_back(Load::BMP("model/world/track/track.bmp"));
	normalMap.push_back(Load::BMP("model/world/track/normal.bmp"));
	indexCount.push_back(Load::meshCount());

	/* Light settings to tweak when using parallax
	maybe adjust materials, light position, attenuation... etc.
	*/
	float quadratic = 0.01f;
	
	left.position = glm::vec4(-1000.0f, 1000.0f, -1000.0f, 1.0f);
	left.constant = 0.0f;
	left.linear = 0.0f;
	left.quadratic = quadratic;
	right.position = glm::vec4(-1000.0f, 1000.0f, 1250.0f, 1.0f);
	right.constant = 0.0f;
	right.linear = 0.0f;
	right.quadratic = quadratic;
	up.position = glm::vec4(1900.0f, 1000.0f, 1250.0f, 1.0f);
	up.constant = 0.0f;
	up.linear = 0.0f;
	up.quadratic = quadratic;
	down.position = glm::vec4(1900.0f, 1000.0f, -1000.0f, 1.0f);
	down.constant = 0.0f;
	down.linear = 0.0f;
	down.quadratic = quadratic;
	material.Ka = glm::vec4(1.0f);
}

bool enabled = true;
float offset = 0.00f;
float scale = 0.02f;

void World::Render()
{
	
	Shader::Push();
	{
		/* Light tweaking here too, depending how many lights to send 
		into the shader
		also here select Shader::Bump() for parallax or
		Shader::DiffusePerVertex() for omnilight with texture
		*/
		Shader::Use(Shader::DiffusePerVertex());
		//Shader::AddLight(left, "light[0]");
		Shader::AddLight(right, "light[1]");
		//Shader::AddLight(up, "light[2]");
		//Shader::AddLight(down, "light[3]");
		Shader::AddMaterial(material);

		Shader::SetUniform("ModelViewMatrix",Shader::ModelViewMatrix());
		Shader::SetUniform("ProjectionMatrix", Shader::ProjectionMatrix());
		Shader::SetUniform("NormalMatrix", Shader::NormalMatrix());
		Shader::SetUniform("MVP", Shader::ProjectionMatrix()*Shader::Top());
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	
		for (int i = 0; i < NUMBEROFOBJECTS; i++)

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