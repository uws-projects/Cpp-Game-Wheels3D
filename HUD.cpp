#include "HUD.h"

#define RED 1
#define YELLOW 2
#define GREEN 3

void HUD::Initialize()
{
	plane = Load::Obj("model/cubeModel.obj");
	indexCount = Load::meshCount();
	text = "0";
	SpeedTexture = Load::Text(text.c_str());
	text = "00:00:000";
	TimerTexture = Load::Text(text.c_str());
	text = "N";
	GearTexture = Load::Text(text.c_str());
	BackgroundTexture = Load::PNG("model/hud.png");
	showStopLight = false;
	StopLight[0] = Load::PNG(".\\model\\stoplight\\0.png");
	StopLight[1] = Load::PNG(".\\model\\stoplight\\1.png");
	StopLight[2] = Load::PNG(".\\model\\stoplight\\2.png");
	StopLight[3] = Load::PNG(".\\model\\stoplight\\3.png");
	lamp = 0;
}

void HUD::Update()
{
	// building Speed texture
	{ 
		int s = (*speed);  
		text = std::to_string(abs(s)); 
		SpeedTexture = Load::Text(text.c_str()); 
	}

	float now = SDL_GetTicks();
	// building Timer texture
	{
		double t = (*timer);
		if (t != 0)	// if the race has started, so time elapsed is not 0
		{
			if (showStopLight == false)
			{
				if (SDL_GetTicks() > t - 6000 && SDL_GetTicks() < t)
					showStopLight = true;
			}
			else
			{
				if (SDL_GetTicks() > t - 2000) lamp = RED;
				if (SDL_GetTicks() > t - 1000) lamp = YELLOW;
				if (SDL_GetTicks() > t ) lamp = GREEN;
				if (SDL_GetTicks() > t + 1000) showStopLight = false;
			}

			// get the time difference in milliseconds 
			double startTime = SDL_GetTicks() - t;// ;
			if (startTime > 0)
			{
				// and format it to this 00:00:000
				// if the number of minutes or seconds is smaller than 10 add a 0
				// before the unit number so we will have 01:03:235 instead of 1:3:235;
				int minutes = (int)startTime / 60000;
				int seconds = (int)(startTime - (minutes * 60000)) / 1000;
				int mili = (int)(startTime - minutes * 60000 - seconds * 1000);
				std::string result = "";
				if (minutes < 10)
				{
					result += "0" + std::to_string(minutes) + ":";
				}
				else
				{
					result += std::to_string(minutes) + ":";
				}
				if (seconds < 10)
				{
					result += "0" + std::to_string(seconds) + ":";
				}
				else
				{
					result += std::to_string(seconds) + ":";
				}
				// add all the formated text in a string
				result += std::to_string(mili);

				// and build the texture based on that string
				TimerTexture = Load::Text(result.c_str());
			}

		}

	}
	// building gear texture
	{
		int g = (*gear);
		
		switch (g)
		{
		case 0: text = "N";
			break;
		case 7: text = "R";
			break;
		default:
			text = std::to_string(g);
			break;
		}
		GearTexture = Load::Text(text.c_str());
	}
};


void HUD::Render()
{
	Shader::Push(Shader::Identity());
	{
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);

		// setup shader
		Shader::Use(Shader::ScreenHUD());
		Shader::AddLight(light);
		Shader::AddMaterial(material);

		// draw background first
		Shader::Push();
		{
			Shader::Bind(0, "tex", BackgroundTexture);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0.0f, 0.0f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(1.0f, 1.0f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw speed first using the same plane model but with 2 different textures
		Shader::Push();
		{
			Shader::Bind(0, "tex", SpeedTexture);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.86f, -0.75f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.03f, 0.1f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw timer now
		Shader::Push();
		{
			Shader::Bind(0, "tex", TimerTexture);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0.858f, 0.768f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.085f, 0.05f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw gear now
		Shader::Push();
		{
			Shader::Bind(0, "tex", GearTexture);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.738f, -0.87f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.02f, 0.05f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		if (showStopLight)
		{
			Shader::Bind(0, "tex", StopLight[lamp]);
			Shader::Push();
			{
				Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0.0f, 0.6f, 0.1f));
				Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.08f, 0.3f, 0.1f));
				Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
				Shader::SetUniform("ModelViewMatrix", Shader::Top());
				glBindVertexArray(plane);
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			Shader::Pop();
		}
	}
	Shader::Pop();
	glEnable(GL_DEPTH_TEST);
}


HUD::~HUD()
{
}
