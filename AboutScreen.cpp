#include "AboutScreen.h"
#include "Application.h"

bool AboutScreen::OnEnter()
{
	SOUND->PauseMusic();
	SOUND->Music(MUSICCREDITS, MusicVolume);
	texture = Load::PNG(".\\model\\menu\\AboutMenu.png");
	model = Load::Obj(".\\model\\cubeModel.obj");
	indexCount = Load::meshCount();
	// create rectangle to cover screen based on one scalable variable

	return true;
}

void AboutScreen::Update()
{

}

void AboutScreen::Render()
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
	}
	Shader::Pop();
}

void AboutScreen::HandleEvents()
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

