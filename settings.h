#pragma once

//#define FULLSCREEN true
#define FULLSCREEN false



#include <GL\glew.h>
#include <SDL.h>
#include <math.h>
#include <string>

#include "Load.h"
#include "Shader.h"
#include "InputHandler.h"

#include <iostream>
#include "Light.h"
#include "Cube.h"
#include "Material.h"

#ifndef RADIANS
#define RADIANS 0.017453293
#endif

#ifndef WIDTH					// width of the screen
#define WIDTH 1680.0f			
#endif

#ifndef HEIGHT					// height of the screen
#define HEIGHT 945.0f
#endif

#ifndef CENTER					// used for centering window position
#define CENTER -1				// improves readability
#endif

#ifndef XCENTRE					// centre of the screen (horizontally)
#define XCENTRE WIDTH/2
#endif

#ifndef YCENTRE					// centre of the screen (vertically)
#define YCENTRE HEIGHT/2
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

#define SHIELD 0
#define HAPPYWHEEL 1
#define TURBO 2
#define REPAIR 3
#define REVERSECONTROLS 4
#define SENSIBILITY 5
#define INSTANTSTOP 6

// joystick buttons
#define BUTTON_A 10
#define BUTTON_B 11
#define BUTTON_X 12
#define BUTTON_Y 13
#define LB 8
#define RB 9
#define BUTTON_SELECT 5
#define BUTTON_START 4
#define LS 6
#define RS 7
#define JOYSTICK1 0
#define JOYSTICK2 1
#define LEFTSTICK 1
#define RIGHTSTICK 2



#define VALUES Settings::Instance()

enum options {
	Volume_Sound_Effects,
	Volume_Engine,
	Volume_Music,
	Effect_Damage,
	Effect_Collision_Tolerance,
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

class Settings
{
public:
	static Settings* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Settings();
			return s_pInstance;
		}
		return s_pInstance;
	}
	~Settings() {}
	
	inline void increase(int option) {
		switch (option) 
		{
		case 0: increaseVolumeSoundEffects(); break;
		case 1: increaseVolumeEngine(); break;
		case 2: increaseVolumeMusic(); break;
		case 3: increaseDamage(); break;
		case 4: increaseCollisionTolerance(); break;
		case 5: increaseCornering(); break;
		case 6: increaseSensibility(); break;
		case 7: increaseTurbo(); break;
		case 8: increaseDurationHappyWheel(); break;
		case 9: increaseDurationSensibility(); break;
		case 10: increaseDurationTurbo(); break;
		case 11: increaseDurationReverseControls(); break;
		case 12: increaseDurationShield(); break;
		default:
			break;
		}
	}

	inline void decrease(int option) {
		switch (option)
		{
		case 0: decreaseVolumeSoundEffects(); break;
		case 1: decreaseVolumeEngine(); break;
		case 2: decreaseVolumeMusic(); break;
		case 3: decreaseDamage(); break;
		case 4: decreaseCollisionTolerance(); break;
		case 5: decreaseCornering(); break;
		case 6: decreaseSensibility(); break;
		case 7: decreaseTurbo(); break;
		case 8: decreaseDurationHappyWheel(); break;
		case 9: decreaseDurationSensibility(); break;
		case 10: decreaseDurationTurbo(); break;
		case 11: decreaseDurationReverseControls(); break;
		case 12: decreaseDurationShield(); break;
		default:
			break;
		}
	}

	inline void reset(int option)	{ settings[option] = defaults[option]; }
	inline void reset()				{ for (int i = 0; i < 13; i++) reset(i); }

	static float settings[13];
	static float defaults[13];

private:
	Settings()
	{
	}
	static Settings* s_pInstance;
	inline void increaseVolumeSoundEffects()		{ if (settings[0] < 0.99f) settings[0] += 0.01f; }
	inline void increaseVolumeEngine()				{ if (settings[1] < 0.19f) settings[1] += 0.01f; }
	inline void increaseVolumeMusic()				{ if (settings[2] < 0.9889f) settings[2] += 0.01f; }

	inline void increaseDamage()					{ if (settings[3] < 0.30f) settings[3] += 0.01f; }
	inline void increaseCollisionTolerance()		{ if (settings[4] < 0.99f) settings[4] += 0.01f; }
	inline void increaseCornering()					{ if (settings[5] < 4.99f) settings[5] += 0.01f; }
	inline void increaseSensibility()				{ if (settings[6] < 0.49f) settings[6] += 0.01f; }
	inline void increaseTurbo()						{ if (settings[7] < 4.99f) settings[7] += 0.01f; }

	inline void increaseDurationHappyWheel()		{ if (settings[8] < 20000.0f) settings[8] += 200.0f; }
	inline void increaseDurationSensibility()		{ if (settings[9] < 20000.0f) settings[9] += 200.0f; }
	inline void increaseDurationTurbo()				{ if (settings[10] < 10000.0f) settings[10] += 200.0f; }
	inline void increaseDurationReverseControls()	{ if (settings[11] < 10000.0f) settings[11] += 200.0f; }
	inline void increaseDurationShield()			{ if (settings[12] < 20000.0f) settings[12] += 200.0f; }

	inline void decreaseVolumeSoundEffects()		{ if (settings[0] > 0.01f) settings[0] -= 0.01f; }
	inline void decreaseVolumeEngine()				{ if (settings[1] > 0.01f) settings[1] -= 0.01f; }
	inline void decreaseVolumeMusic()				{ if (settings[2] > 0.01f) settings[2] -= 0.01f; }

	inline void decreaseDamage()					{ if (settings[3] > 0.01f) settings[3] -= 0.01f; }
	inline void decreaseCollisionTolerance()		{ if (settings[4] > 0.01f) settings[4] -= 0.01f; }
	inline void decreaseCornering()					{ if (settings[5] > 0.01f) settings[5] -= 0.01f; }
	inline void decreaseSensibility()				{ if (settings[6] > 0.01f) settings[6] -= 0.01f; }
	inline void decreaseTurbo()						{ if (settings[7] > 0.01f) settings[7] -= 0.01f; }

	inline void decreaseDurationHappyWheel()		{ if (settings[8] > 1200.0f) settings[8] -= 200.0f; }
	inline void decreaseDurationSensibility()		{ if (settings[9] > 1200.0f) settings[9] -= 200.0f; }
	inline void decreaseDurationTurbo()				{ if (settings[10] > 1200.0f) settings[10] -= 200.0f; }
	inline void decreaseDurationReverseControls()	{ if (settings[11] > 1200.0f) settings[11] -= 200.0f; }
	inline void decreaseDurationShield()			{ if (settings[12] > 1200.0f) settings[12] -= 200.0f; }
};
