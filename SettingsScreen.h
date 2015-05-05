#pragma once
#include "AbstractState.h"
#include "Sound.h"

class SettingsScreen :
	public AbstractState
{
public:
	SettingsScreen() {}
	~SettingsScreen() {}

	void Update();
	void Render();
	void HandleEvents();

	bool OnEnter();
	bool OnExit() { SOUND->PauseMusic(); SOUND->Music(MUSICMENU, VALUES->settings[Volume_Music]); return true; }

private:

	struct Option {
		GLuint idle_texture;
		GLuint active_texture;
		GLuint texture;
		std::string label;
		int length;
	};

	GLuint texture;
	GLuint model;
	Light light;
	Material material;
	int indexCount;
	std::string name;
	void displayOption(float x, float y, Option option);				// method to display option at x, y position
	void displayArrow(float x, float y, GLuint texture);
	void displayString(float x, float y, std::string text, int r, int g, int b);	// method to display string at x, y position
	const int NUMBER_OF_OPTIONS = 14;
	int selected;
	
	// will hold the option number

	Option option[14];

	// cursor movement methods
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void increaseValue();
	void decreaseValue();

	GLuint leftArrow[14];		// will point to the right texture (active or idle) for each of the 14 left arrows
	GLuint rightArrow[14];		// will point to the right texture (active or idle) for each of the 14 right arrows
	GLuint valueOf[14];			// will hold a texture with the value of each option
	GLuint left_arrow_idle;		// texture for idle left arrow
	GLuint left_arrow_active;	// texture for active left arrow
	GLuint right_arrow_idle;	// texture for idle right arrow
	GLuint right_arrow_active;	// texture for active right arrow
	bool leftPressed, rightPressed;
	GLuint soundTexture, powerTexture, durationTexture;		// will hold a texture for a text displayed on the settings screen
	int axisValue;				// this will record the up down joystick movement	
	bool move = true;			// this will allow or not the option to scroll up/ down
};

