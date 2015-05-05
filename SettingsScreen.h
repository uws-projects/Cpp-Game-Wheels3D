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
	bool OnExit() { SOUND->PauseMusic(); SOUND->Music(MUSICMENU, MusicVolume); return true; }

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
	void displayString(float x, float y, float width, float height, std::string text, int r, int g, int b);	// method to display string at x, y position
	const int NUMBER_OF_OPTIONS = 13;
	int selected;
	
	enum options {
		Volume_Sound_Effects,
		Volume_Engine,
		Volume_Music,
		Effect_Damage,
		Effect_Cornering,
		Effect_Sensibility,
		Effect_Turbo,
		Duration_HappyWheel,
		Duration_Sensibility,
		Duration_Turbo,
		Duration_Reverse_Controls,
		Duration_Shield,
		Reset_All_Default
	};
	// will hold the option number

	Option option[13];

	// cursor movement methods
	void moveUp();
	void moveDown();
	void moveLeft(int option);
	void moveRight(int option);

};

