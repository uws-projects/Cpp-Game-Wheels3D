#include "settings.h"

Settings* Settings::s_pInstance = 0;

float Settings::defaults[13] = {
	0.2f,		// Volume_Sound_Effects,
	0.05f,		// Volume_Engine,
	0.5f,		// Volume_Music,
	0.05f,		// Effect_Damage,
	0.95f,		// Effect_Collision_Tolerance,
	2.0f,		// Effect_Cornering,
	0.3f,		// Effect_Sensibility,
	2.0f,		// Effect_Turbo,
	10000.0f,	// Duration_HappyWheel,
	10000.0f,	// Duration_Sensibility,
	3000.0f,	// Duration_Turbo,
	3000.0f,	// Duration_Reverse_Controls,
	5000.0f		// Duration_Shield,
};
float Settings::settings[13] = {
	0.2f,		// Volume_Sound_Effects,
	0.05f,		// Volume_Engine,
	0.5f,		// Volume_Music,
	0.05f,		// Effect_Damage,
	0.95f,		// Effect_Collision_Tolerance,
	2.0f,		// Effect_Cornering,
	0.3f,		// Effect_Sensibility,
	2.0f,		// Effect_Turbo,
	10000.0f,	// Duration_HappyWheel,
	10000.0f,	// Duration_Sensibility,
	3000.0f,	// Duration_Turbo,
	3000.0f,	// Duration_Reverse_Controls,
	5000.0f		// Duration_Shield,
};

