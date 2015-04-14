#include "World.h"

World::World()
{
}

const int NUMBEROFOBJECTS = 17;

void World::Initialize()
{
	models.push_back(Load::Obj("model/world/asphalt.map"));
	textures.push_back(Load::PNG("model/world/asphalt/asphalt.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/grass.map"));
	textures.push_back(Load::PNG("model/world/grass/grass.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/billbloard_buildings.map"));
	textures.push_back(Load::PNG("model/world/billbloard_buildings/billbloard_buildings.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/billbloard_buildings_roofs.map"));
	textures.push_back(Load::PNG("model/world/billbloard_buildings_roofs/billbloard_buildings_roofs.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/brick.map"));
	textures.push_back(Load::PNG("model/world/brick/brick.png"));
	indexCount.push_back(Load::meshCount());

	// TO DO: create normal map for connector and create textures
	models.push_back(Load::Obj("model/world/connector.map"));
	textures.push_back(Load::PNG("model/world/connector/connector.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/field.map"));
	textures.push_back(Load::PNG("model/world/field/field.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/floor.map"));
	textures.push_back(Load::PNG("model/world/floor/floor.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/ground_floor.map"));
	textures.push_back(Load::PNG("model/world/ground_floor/ground_floor.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/ground_and_first_floor.map"));
	textures.push_back(Load::PNG("model/world/ground_and_first_floor/ground_and_first_floor.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/passage.map"));
	textures.push_back(Load::PNG("model/world/passage/passage.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/pvcwindow.map"));
	textures.push_back(Load::PNG("model/world/pvcwindow/pvcwindow.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/roof.map"));
	textures.push_back(Load::PNG("model/world/roof/roof.png"));
	indexCount.push_back(Load::meshCount());

	// TO DO: normal map for track
	models.push_back(Load::Obj("model/world/track.map"));
	textures.push_back(Load::PNG("model/world/track/track.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/road_signs.map"));
	textures.push_back(Load::PNG("model/world/road_signs/road_signs.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/start_finish.map"));
	textures.push_back(Load::PNG("model/world/start_finish/start_finish.png"));
	indexCount.push_back(Load::meshCount());

	models.push_back(Load::Obj("model/world/test.obj"));
	textures.push_back(Load::PNG("model/world/margin/margin.png"));
	indexCount.push_back(Load::meshCount());

	quadratic = 0.06f;
	linear = 0.0f;
	constant = 1.0f;
	
	left.position = glm::vec4(-95.0f, 1000.0f, -1000.0f, 1.0f);
	left.constant = constant;
	left.linear = linear;
	left.quadratic = quadratic;
	right.position = glm::vec4(-95.0f, 1000.0f, 2250.0f, 1.0f);
	right.constant = constant;
	right.linear = linear;
	right.quadratic = quadratic;
	up.position = glm::vec4(1900.0f, 1000.0f, 125.0f, 1.0f);
	up.constant = constant;
	up.linear = linear;
	up.quadratic = quadratic;
	down.position = glm::vec4(-1900.0f, 1000.0f, -125.0f, 1.0f);
	down.constant = constant;
	down.linear = linear;
	down.quadratic = quadratic;
	material.Ka = glm::vec4(1.0f);
}

void World::Render()
{
	left.quadratic = quadratic;
	right.quadratic = quadratic;
	up.quadratic = quadratic;
	down.quadratic = quadratic;

	Shader::Push();
	{
		/* Light tweaking here too, depending how many lights to send 
		into the shader
		also here select Shader::Bump() for parallax or
		Shader::DiffusePerVertex() for omnilight with texture
		*/
		Shader::Use(Shader::DiffusePerVertex());
		Shader::AddLight(left,	"light[0]");
		Shader::AddLight(right, "light[1]");
		Shader::AddLight(up,	"light[2]");
		Shader::AddLight(down,	"light[3]");
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
			
			// draw
			glBindVertexArray(models[i]);
			glDrawElements(GL_TRIANGLES, indexCount[i], GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
	Shader::Pop();
}