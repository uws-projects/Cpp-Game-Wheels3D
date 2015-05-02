#pragma once
#ifndef __Sound__
#define __Sound__

#include "bass.h"
#include <iostream>

#define PLAY Sound::Instance()->playSound
#define PAUSE Sound::Instance()->pauseSound
#define SOUND Sound::Instance()

#define SELECTSOUND 0
#define BACKSOUND 1
#define ACCEPTSOUND 2
#define QUITSOUND 3
#define ENGINELOOP 4

#define MUSICTITLE 0
#define MUSICSETUP 1
#define MUSICSCOREBOARD 2
#define MUSICMENU 3
#define MUSICCREDITS 4
#define MUSICHIGHSCORE 5

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

	int randomTo(int x) { return rand() % x; }

	void Play(HSAMPLE &sound);
	void Play(int soundNumber) { Play(soundList[soundNumber]); }
	void Play(int soundNumber, float volume) { Play(soundList[soundNumber], volume); }
	void Play(int soundNumber, float volume, float pitch) { Play(soundList[soundNumber], volume, pitch); }
	void Play(HSAMPLE &sound, float volume);
	void Play(HSAMPLE &sound, float volume, float pitch);
	void Music(int soundNumber, float volume) { currentMusic = musicList[soundNumber]; Play(currentMusic, volume); }
	void Pause(HSAMPLE &sound);
	void Pause(int soundNUmber) { Pause(soundList[soundNUmber]); }
	void PauseMusic();
	void Resume();
	void Happy(float volume) { Play(happy[randomTo(5)], volume); }
	void Repair(float volume) { Play(repair[randomTo(3)], volume); }
	void Reverse(float volume) { Play(reverse[randomTo(5)], volume); }
	void Sensibility(float volume) { Play(sensibility[randomTo(4)], volume); }
	void Shield(float volume) { Play(shield[randomTo(4)], volume); }
	void Stop(float volume) { Play(stop[randomTo(4)], volume); }
	void Turbo(float volume) { Play(turbo[randomTo(4)], volume); }
	static HSAMPLE Load(char* filename);

private:
	Sound()
	{
		if (!BASS_Init(-1, 44100, 0, 0, 0)) std::cerr << "Can`t initialize sound device\n";
	}
	static void Initialize();

	static Sound* s_pInstance;
	static HSAMPLE musicList[6];
	static HSAMPLE soundList[5];
	static HSAMPLE effectList[5];
	static HSAMPLE happy[5];
	static HSAMPLE repair[3];
	static HSAMPLE reverse[5];
	static HSAMPLE sensibility[4];
	static HSAMPLE shield[4];
	static HSAMPLE stop[4];
	static HSAMPLE turbo[4];
	static HSAMPLE currentMusic;
};

#endif // defined (__SoundManager__)