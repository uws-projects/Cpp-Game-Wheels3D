#include "HighScoreScreen.h"
#include "Application.h"
#include "Menu.h"

bool HighScoreScreen::OnEnter()
{
	SOUND->PauseMusic();
	SOUND->Music(MUSICHIGHSCORE, MusicVolume);
	texture = Load::PNG(".\\model\\menu\\HighscoreMenu.png");
	model = Load::Obj(".\\model\\cubeModel.obj");
	indexCount = Load::meshCount();
	// create rectangle to cover screen based on one scalable variable

	for (int i = 0; i < 10; i++) {
		playerTextures[i] = Load::Text(SCORE->GetPlayer(i).c_str(), false);
		scoreTextures[i] = Load::Text(SCORE->GetScore(i).c_str());
	}
	return true;
}

void HighScoreScreen::Update()
{

}

float x = -0.1f, y = 0.385f, distX = 0.0f, distY = 0.12f;
float sx = 0.06f, sy = 0.04f;

void HighScoreScreen::Render()
{
	Shader::Push(Shader::Identity());
	{
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		

		// setup shader
		Shader::Use(Shader::ScreenHUD());
		Shader::AddLight(light);
		Shader::AddMaterial(material);
		Shader::Push();
		{
			// draw background
			Shader::Bind(0, "tex", texture);
			Shader::Top() = glm::translate(Shader::Top(), glm::vec3(0, 0, 0.1f));
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(1.0f,1.0f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(model);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		for (int i = 0; i < 10; i++)
		{
			Shader::Push();
			{
				// draw player name texture
				Shader::Bind(0, "tex", playerTextures[i]);
				Shader::Top() = glm::translate(Shader::Top(), glm::vec3(x, y - distY * i, 0.1f));
				Shader::Top() = glm::scale(Shader::Top(), glm::vec3(sx, sy, 0.1f));
				Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
				Shader::SetUniform("ModelViewMatrix", Shader::Top());
				glBindVertexArray(model);
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			Shader::Pop();
		}

		for (int i = 0; i < 10; i++)
		{
			Shader::Push();
			{
				// draw player time texture
				Shader::Bind(0, "tex", scoreTextures[i]);
				Shader::Top() = glm::translate(Shader::Top(), glm::vec3(x+ 0.26f, y - distY * i, 0.1f));
				Shader::Top() = glm::scale(Shader::Top(), glm::vec3(sx, sy, 0.1f));
				Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
				Shader::SetUniform("ModelViewMatrix", Shader::Top());
				glBindVertexArray(model);
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			Shader::Pop();
		}
	}
	Shader::Pop();
}

void HighScoreScreen::HandleEvents()
{
	// if we have joystick
	// we can skip to next state by pressing either escape or start button on gamepad
	if (JOY->JoysticksInitialised())
	{
		if (JOY_B || JOY_START || PRESSING(SDL_SCANCODE_ESCAPE))
		{
			SOUND->Play(SELECTSOUND, SampleVolume);
			SDL_Delay(100);
			Application::Instance()->GetStateMachine()->PopState();
		}
		if (JOY_X)
		{
			SCORE->Reset();
			for (int i = 0; i < 10; i++) {
				playerTextures[i] = Load::Text(SCORE->GetPlayer(i).c_str(), false);
				scoreTextures[i] = Load::Text(SCORE->GetScore(i).c_str());
			}
		}
	}
	else
		// else we can skip it only by pressing escape
	{
		if (PRESSING(SDL_SCANCODE_ESCAPE))
		{
			SOUND->Play(SELECTSOUND, SampleVolume);
			Application::Instance()->GetStateMachine()->PopState();
		}
	}

}

