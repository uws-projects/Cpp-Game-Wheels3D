#pragma once
#ifndef __Sound__
#define __Sound__

#include "bass.h"
#include <iostream>

#define PLAY Sound::Instance()->playSound
#define PAUSE Sound::Instance()->pauseSound
#define SOUND Sound::Instance()
#define NUMBEROFSOUNDS 1
#define INTROSOUND 0

// sound list

class Sound
{
public:
	static Sound* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Sound();
			Initialize();
			return s_pInstance;
		}
		return s_pInstance;
	}
	~Sound() {}

	
	void Play(HSAMPLE &sound);
	void Play(int soundNumber) { Play(soundlist[soundNumber]); }
	void Play(int soundNumber, float volume) { Play(soundlist[soundNumber], volume); }
	void Play(HSAMPLE &sound, float volume);
	void Pause(HSAMPLE &sound);
	void Resume();
	static HSAMPLE Load(char* filename);

private:
	Sound()
	{
		if (!BASS_Init(-1, 44100, 0, 0, 0)) std::cerr << "Can`t initialize sound device\n";
	}
	static void Initialize();
	
	static Sound* s_pInstance;
	static HSAMPLE soundlist[NUMBEROFSOUNDS];
};

#endif // defined (__SoundManager__)