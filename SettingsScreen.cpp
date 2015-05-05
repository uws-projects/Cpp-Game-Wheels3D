#include "SettingsScreen.h"
#include "Application.h"
#include <conio.h>
#include <iostream>
#include <iomanip>

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

// decreases the value of the currently selected option
// and updates the texture for the arrow
void SettingsScreen::moveLeft() {
	leftArrow[selected] = left_arrow_active;
	VALUES->decrease(selected);
	SDL_Delay(10);
}

// increases the value of the currently selected option
// and updates the texture for the arrow
void SettingsScreen::moveRight() {
	rightArrow[selected] = right_arrow_active;
	VALUES->increase(selected);
	SDL_Delay(10);
}

bool SettingsScreen::OnEnter()
{
	SOUND->PauseMusic();
	SOUND->Music(MUSICCREDITS, VALUES->settings[Volume_Music]);
	texture = Load::PNG(".\\model\\menu\\seetingsMenu.png");
	model = Load::Obj(".\\model\\cubeModel.obj");
	indexCount = Load::meshCount();
	// create rectangle to cover screen based on one scalable variable
	name = "Player";
	selected = 0;
	leftPressed = false;
	rightPressed = false;

	option[Volume_Sound_Effects].label = "Effects";
	option[Volume_Engine].label = "Engine";
	option[Volume_Music].label = "Music";
	option[Effect_Damage].label = "Damage";
	option[Effect_Collision_Tolerance].label = "Collision Tolerance";
	option[Effect_Cornering].label = "Turning speed";
	option[Effect_Sensibility].label = "Sensibility";
	option[Effect_Turbo].label = "Turbo";
	option[Duration_HappyWheel].label = "Happy Wheel";
	option[Duration_Sensibility].label = "Sensibility";
	option[Duration_Turbo].label = "Turbo";
	option[Duration_Reverse_Controls].label = "Reverse Controls";
	option[Duration_Shield].label = "Shield";
	option[Reset_All_Default].label = "Reset all";


	left_arrow_idle = Load::Text("-", 0, 255, 255);
	left_arrow_active = Load::Text("-", 0, 255, 0);
	right_arrow_idle = Load::Text("+", 0, 255, 255);
	right_arrow_active = Load::Text("+", 0, 255, 0);
	soundTexture = Load::Text("Sound Volume", 255, 255, 255);
	powerTexture = Load::Text("Powerups effects", 255, 255, 255);
	soundTexture = Load::Text("Powerups duration", 255, 255, 255);

	// initialize all textures with texture with colour for unselected items
	for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
		option[i].length = option[i].label.length();
		option[i].idle_texture = Load::Text(option[i].label.c_str(), 0, 255, 255);
		option[i].active_texture= Load::Text(option[i].label.c_str(), 0, 255, 0);
		option[i].texture = option[i].idle_texture;
		leftArrow[i] = left_arrow_idle;
		rightArrow[i] = right_arrow_idle;
	}


	return true;
}

// this function displays the option at x, y coordinates and passes through an Option struct
// which holds the texture and the length of the text to be rendered
void SettingsScreen::displayOption(float x, float y, Option option) {
	Shader::Push();
	{
		Shader::Bind(0, "tex", option.texture);
		// convert positions from -1,1 interval to pixels from 0 to screen wdith/height
		Shader::Top() = glm::translate(Shader::Top(), glm::vec3((x - WIDTH / 2) / (WIDTH / 2) + option.length*0.01f, (HEIGHT / 2 - y) / (HEIGHT / 2), 0.1f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(option.length * (0.01f), 0.03f, 0.1f));
		Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		glBindVertexArray(model);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	Shader::Pop();
}

// this function displays an arrow at x, y position, just by using the passed texture 
// no memory leaking
void SettingsScreen::displayArrow(float x, float y, GLuint texture) {
	Shader::Push();
	{
		Shader::Bind(0, "tex", texture);
		// convert positions from -1,1 interval to pixels from 0 to screen wdith/height
		Shader::Top() = glm::translate(Shader::Top(), glm::vec3((x - WIDTH / 2) / (WIDTH / 2) + 0.01f, (HEIGHT / 2 - y) / (HEIGHT / 2), 0.1f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.01f, 0.04f, 0.1f));
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
void SettingsScreen::displayString(float x, float y, std::string text, int r, int g, int b) {
	GLuint texture = Load::Text(text.c_str(), r, g, b);

	Shader::Push();
	{
		Shader::Bind(0, "tex", texture);
		// convert positions from -1,1 interval to pixels from 0 to screen wdith/height
		Shader::Top() = glm::translate(Shader::Top(), glm::vec3((x - ((float) (text.length() / 2.0f) * 0.01f) - WIDTH / 2) / (WIDTH / 2), (HEIGHT / 2 - y) / (HEIGHT / 2), 0.1f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(text.length()*0.01f, 0.04f, 0.1f));
		Shader::SetUniform("ProjectionMatrix", glm::mat4(1.0f));
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		glBindVertexArray(model);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	Shader::Pop();
}

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
			if (leftPressed) leftArrow[i] = left_arrow_active; else leftArrow[i] = left_arrow_idle;
			if (rightPressed) rightArrow[i] = right_arrow_active; else rightArrow[i] = right_arrow_idle;
		}
		else {
			option[i].texture = option[i].idle_texture;
			leftArrow[i] = left_arrow_idle;
			rightArrow[i] = right_arrow_idle;
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

		float distance = HEIGHT*0.19f;

		for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
			// Sound related options
			displayOption(WIDTH*0.3f, distance + i * 35.0f, option[i]);
			// when we are drawing the reset option, we don`t need any arrows
			// so we might as well exit the loop
			if (i == Reset_All_Default) {
				break;
			}
			displayArrow(WIDTH*0.60f, distance + i * 35.0f, leftArrow[i]);
			std::string text;
			if (i < 8)
			{
				float value = VALUES->settings[i];
				text = std::to_string(value);
				text = text.substr(0, 4);
			}
			if (i > 7)
			{
				int value = (int) VALUES->settings[i]/1000;
				text = std::to_string(value);
			}
			displayString(WIDTH*0.65f, distance + i * 35.0f, (text.c_str()), 0, 255, 255);
			displayArrow(WIDTH*0.7f, distance + i * 35.0f, rightArrow[i]);

			// we leave a space after these options, these separate the audio 
			// from effects and from duration
			if (i == Volume_Music || i == Effect_Turbo || i == Duration_Shield) distance += 37.0f;
			
		}

		displayString(WIDTH*0.5f, distance - 4.0f*35.0f, "Sound Volume", 255, 255, 255);
		displayString(WIDTH*0.5f, distance, "Powerups effect", 255, 255, 255);
		displayString(WIDTH*0.5f, distance + 6.0f * 35.0f, "Powerups duration", 255, 255, 255);

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
			SOUND->Play(SELECTSOUND, VALUES->settings[Volume_Sound_Effects]);
			SDL_Delay(100);
			Application::Instance()->GetStateMachine()->PopState();
		}

		// if the joystick is in the same position, don`t record movement
		// else scroll up/down
		if (axisValue == LEFTYAXIS100) {
			move = false;
		}
		else {
			move = true;
		}

		if (move && axisValue > 98) {
			moveDown();
		}

		if (move && axisValue < -98) {
			moveUp();
		}

		if (JOY_RB) {
			rightPressed = true;
			moveRight();
		}
		else {
			rightPressed = false;
		}

		if (JOY_LB) {
			leftPressed = true;
			moveLeft();
		}
		else {
			leftPressed = false;
		}

		if (JOY_Y)
		{
			if (selected == Reset_All_Default) {
				VALUES->reset();
			}
			else
			{
				VALUES->reset(selected);
			}
		}

		axisValue = LEFTYAXIS100;
	}
	else
		// else we can skip it only by pressing escape
	{
		if (PRESSING(SDL_SCANCODE_ESCAPE))
		{
			SOUND->Play(SELECTSOUND, VALUES->settings[Volume_Sound_Effects]);
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

		if (PRESSING(SDL_SCANCODE_LEFT))
		{
			leftPressed = true;
			moveLeft();
		}
		else {
			leftPressed = false;
		}
		if (PRESSING(SDL_SCANCODE_RIGHT))
		{
			rightPressed = true;
			moveRight();
		}
		else {
			rightPressed = false;
		}

		if (PRESSING(SDL_SCANCODE_RETURN))
		{
			VALUES->reset();
		}
	}

}

