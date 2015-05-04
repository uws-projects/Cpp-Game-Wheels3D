#include "SettingsScreen.h"
#include "Application.h"
#include <conio.h>

bool SettingsScreen::OnEnter()
{
	SOUND->PauseMusic();
	SOUND->Music(MUSICCREDITS, MusicVolume);
	//texture = Load::PNG(".\\model\\menu\\AboutMenu.png");
	texture = Load::PNG(".\\model\\background.png");
	model = Load::Obj(".\\model\\cubeModel.obj");
	indexCount = Load::meshCount();
	// create rectangle to cover screen based on one scalable variable
	name = "Player";
	return true;
}

void SettingsScreen::displayString(float x, float y, float width, float height, std::string text, int r, int g, int b) {
	GLuint texture = Load::Text(text.c_str(), r, g, b);

	Shader::Push();
	{
		Shader::Bind(0, "tex", texture);
		// convert positions from -1,1 interval to pixels from 0 to screen wdith/height
		Shader::Top() = glm::translate(Shader::Top(), glm::vec3((x-WIDTH/2)/(WIDTH/2), (HEIGHT/2-y)/(HEIGHT/2), 0.1f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(width/WIDTH, height/HEIGHT, 0.1f));
		Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		glBindVertexArray(model);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	Shader::Pop();
	glDeleteTextures(1, &texture);
}

bool read = false;

void SettingsScreen::Update()
{

}

void SettingsScreen::Render()
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
			Shader::Top() = glm::scale(Shader::Top(), glm::vec3(1.0f, 1.0f, 0.1f));
			Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			glBindVertexArray(model);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		Shader::Pop();

		displayString(100, 600, 150, 30, "Option 1", 255, 255, 0);
		displayString(100, 700, 150, 30, "Option 2", 0, 255, 255);
		displayString(100, 800, 150, 30, "Option 3", 0, 255, 0);

		displayString(100, 300, 150, 30, "Option 4", 255, 0, 0);
		displayString(100, HEIGHT, 150, 30, "Option 5", 0, 255, 255);
		displayString(840, 472, 150, 30, name, 0, 255, 0);
	}
	Shader::Pop();
}

void SettingsScreen::HandleEvents()
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

