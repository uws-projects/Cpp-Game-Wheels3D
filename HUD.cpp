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
	text = "0%";
	DamageText = Load::Text(text.c_str());
	text = "00:00:000";
	TimerTexture = Load::Text(text.c_str());
	text = "N";
	GearTexture = Load::Text(text.c_str());
	hud[0] = Load::PNG(".\\model\\hud\\hud_released.png");
	hud[1] = Load::PNG(".\\model\\hud\\hud_pressed.png");
	showStopLight = false;
	StopLight[0] = Load::PNG(".\\model\\stoplight\\0.png");
	StopLight[1] = Load::PNG(".\\model\\stoplight\\1.png");
	StopLight[2] = Load::PNG(".\\model\\stoplight\\2.png");
	StopLight[3] = Load::PNG(".\\model\\stoplight\\3.png");
	lamp = 0;
	DamageTexture[0] = Load::PNG(".\\model\\hud\\damage0.png");
	DamageTexture[1] = Load::PNG(".\\model\\hud\\damage1.png");
	DamageTexture[2] = Load::PNG(".\\model\\hud\\damage2.png");
	DamageTexture[3] = Load::PNG(".\\model\\hud\\damage3.png");
	leftLevel = &player->GetLeftLevel();
	rightLevel = &player->GetRightLevel();
	gearStatus = &player->GetGearStatus();


	shield_on = Load::PNG("model/powerups/shield.png");
	shield_off = Load::PNG("model/powerups/shield_off.png");
	happy_on = Load::PNG("model/powerups/happy_wheel.png");
	happy_off = Load::PNG("model/powerups/happy_wheel_off.png");
	turbo_on = Load::PNG("model/powerups/turbo.png");
	turbo_off = Load::PNG("model/powerups/turbo_off.png");
	rev_on = Load::PNG("model/powerups/reverse_controls.png");
	rev_off = Load::PNG("model/powerups/reverse_controls_off.png");
	sens_on = Load::PNG("model/powerups/sensibility.png");
	sens_off = Load::PNG("model/powerups/sensibility_off.png");

	shield = &shield_on;
	happy = &happy_on;
	turbo = &turbo_on;
	rev = &rev_on;
	sens = &sens_on;

}

void HUD::Update()
{
	// updating the powerup textures
	if (player->shield)				shield = &shield_on;	else shield = &shield_off;
	if (player->happyWheel)			happy = &happy_on;		else happy= &happy_off;
	if (player->turbo)				turbo = &turbo_on;		else turbo = &turbo_off;
	if (player->reverseControls)	rev = &rev_on;			else rev = &rev_off;
	if (player->sensibility)		sens = &sens_on;		else sens = &sens_off;

	// building Speed texture
	{
		int s = player->Velocity();
		text = std::to_string(abs(s));
		SpeedTexture = Load::Text(text.c_str());
	}

	// building Damage Text texture
	{
	int d = player->GetDamage();
	text = std::to_string(d);
	text += "%";
	DamageText = Load::Text(text.c_str());
	}

	float now = SDL_GetTicks();
	// building Timer texture
	{
		double t = player->StartTimer();
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
				if (!player->IsRaceFinished()) {
					player->SetFinalTime(startTime);
					TimerTexture = Load::Text(result.c_str());
				}
				else
				{
					std::cout << "Final Time:" << player->RaceResult();
				}

			}

		}

	}
	// building gear texture
	{
		int g = player->Gear();
		
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
			Shader::Bind(0, "tex", hud[*gearStatus]);
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

		// draw damage now
		Shader::Push();
		{
			// left side
			Shader::Bind(0, "tex", DamageTexture[*leftLevel]);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0.818f, -0.74f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.03f, 0.23f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();
		Shader::Push();
		{
			// right side
			Shader::Bind(0, "tex", DamageTexture[*rightLevel]);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0.878f, -0.74f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(-0.03f, 0.23f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();
		Shader::Push();
		{
			// and the damage value
			Shader::Bind(0, "tex", DamageText);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0.848f, -0.73f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.02f, 0.05f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();
		if (showStopLight)
			// draw stoplight
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
		// draw powerup 1 shield
		Shader::Push();
		{
			Shader::Bind(0, "tex", *shield);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.92f, 0.92f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.049f, 0.049f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw powerup 2 happy
		Shader::Push();
		{
			Shader::Bind(0, "tex", *happy);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.82f, 0.92f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.049f, 0.049f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw powerup 3 turbo
		Shader::Push();
		{
			Shader::Bind(0, "tex", *turbo);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.72f, 0.92f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.049f, 0.049f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw powerup 4 rev
		Shader::Push();
		{
			Shader::Bind(0, "tex", *rev);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.62f, 0.92f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.049f, 0.049f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		// draw powerup 5 sens
		Shader::Push();
		{
			Shader::Bind(0, "tex", *sens);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(-0.52f, 0.92f, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.049f, 0.049f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(plane);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();
	}
	Shader::Pop();
	glEnable(GL_DEPTH_TEST);
}


HUD::~HUD()
{
}
