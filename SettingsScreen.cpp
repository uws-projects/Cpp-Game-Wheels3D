#include "SettingsScreen.h"
#include "Application.h"
#include <conio.h>

// updates the option value to make sure that the 
// right option is highlighted when pressing up
void SettingsScreen::moveUp() {
	if (selected == 0) {
		selected = NUMBER_OF_OPTIONS - 1;
	}
	else selected--;
}

// updates the option value to make sure that the 
// right option is highlighted when pressing down
void SettingsScreen::moveDown() {
	if (selected == NUMBER_OF_OPTIONS - 1) {
		selected = 0;
	}
	else selected++;
}



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
	selected = 0;

	option[Volume_Sound_Effects].label = "Effects";
	option[Volume_Engine].label = "Engine";
	option[Volume_Music].label = "Music";
	option[Effect_Damage].label = "Damage";
	option[Effect_Cornering].label = "Turning speed";
	option[Effect_Sensibility].label = "Sensibility";
	option[Effect_Turbo].label = "Turbo";
	option[Duration_HappyWheel].label = "Happy Wheel";
	option[Duration_Sensibility].label = "Sensibility";
	option[Duration_Turbo].label = "Turbo";
	option[Duration_Reverse_Controls].label = "Reverse Controls";
	option[Duration_Shield].label = "Shield";
	option[Reset_All_Default].label = "Reset all";

	for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
		option[i].length = option[i].label.length();
		option[i].idle_texture = Load::Text(option[i].label.c_str(), 0, 255, 255);
		option[i].active_texture= Load::Text(option[i].label.c_str(), 0, 255, 0);
		option[i].texture = option[i].idle_texture;
	}

	return true;
}

void SettingsScreen::displayOption(float x, float y, Option option) {
	Shader::Push();
	{
		Shader::Bind(0, "tex", option.texture);
		// convert positions from -1,1 interval to pixels from 0 to screen wdith/height
		Shader::Top() = glm::translate(Shader::Top(), glm::vec3((x - WIDTH / 2) / (WIDTH / 2) + option.length*0.01f, (HEIGHT / 2 - y) / (HEIGHT / 2), 0.1f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(option.length*0.01f, 0.04f, 0.1f));
		Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		glBindVertexArray(model);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	Shader::Pop();
}


// this function would display a string at x, y screen coordinates with a widht and height in pixels and colour
// the problem is that this creates memory leaks and it will not be used...
void SettingsScreen::displayString(float x, float y, float width, float height, std::string text, int r, int g, int b) {
	GLuint texture = Load::Text(text.c_str(), r, g, b);

	Shader::Push();
	{
		Shader::Bind(0, "tex", texture);
		// convert positions from -1,1 interval to pixels from 0 to screen wdith/height
		Shader::Top() = glm::translate(Shader::Top(), glm::vec3((x - WIDTH / 2) / (WIDTH / 2) + text.length()*0.01f, (HEIGHT / 2 - y) / (HEIGHT / 2), 0.1f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(text.length()*0.01f, 0.04f, 0.1f));
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
	/*
		for all the options we set the colours
			if the option is selected, then it`s colour is green
			otherwise make it different
	*/
	for (int i = 0; i < NUMBER_OF_OPTIONS; i++)
	{
		if (i == selected) {
			option[i].texture = option[i].active_texture;
		}
		else {
			option[i].texture = option[i].idle_texture;
		}
	}
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

		// Sound related options
		displayOption(100, 100, option[Volume_Sound_Effects]);
		displayOption(100, 150, option[Volume_Engine]);
		displayOption(100, 200, option[Volume_Music]);

		// Powerups effect
		displayOption(100, 300, option[Effect_Damage]);
		displayOption(100, 350, option[Effect_Cornering]);
		displayOption(100, 400, option[Effect_Sensibility]);
		displayOption(100, 450, option[Effect_Turbo]);

		// Duration of effect
		displayOption(100, 550, option[Duration_HappyWheel]);
		displayOption(100, 600, option[Duration_Sensibility]);
		displayOption(100, 650, option[Duration_Turbo]);
		displayOption(100, 700, option[Duration_Reverse_Controls]);
		displayOption(100, 750, option[Duration_Shield]);

		displayOption(100, 850, option[Reset_All_Default]);
	
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
		if (PRESSING(SDL_SCANCODE_UP)) {
			moveUp();
			SDL_Delay(150);
		}
		if (PRESSING(SDL_SCANCODE_DOWN))
		{
			moveDown();
			SDL_Delay(150);
		}
	}

}

