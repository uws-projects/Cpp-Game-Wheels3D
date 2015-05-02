#include "Sound.h"

Sound * Sound::s_pInstance = 0;
HSAMPLE Sound::currentMusic = NULL;
HSAMPLE Sound::musicList[6] = {};
HSAMPLE Sound::soundList[5] = {};
HSAMPLE Sound::happy[5] = {};
HSAMPLE Sound::repair[3] = {};
HSAMPLE Sound::reverse[5] = {};
HSAMPLE Sound::sensibility[4] = {};
HSAMPLE Sound::shield[4] = {};
HSAMPLE Sound::stop[4] = {};
HSAMPLE Sound::turbo[4] = {};


void Sound::Initialize()
{


	musicList[MUSICTITLE] = Load(".\\music\\music01.ogg");
	musicList[MUSICCREDITS] = Load(".\\music\\music02.ogg");
	musicList[MUSICSCOREBOARD] = Load(".\\music\\music03.ogg");
	musicList[MUSICSETUP] = Load(".\\music\\music04.ogg");
	musicList[MUSICMENU] = Load(".\\music\\music05.ogg");
	musicList[MUSICHIGHSCORE] = Load(".\\music\\music06.ogg");

	happy[0] = Load(".\\samples\\powerup\\happy0.ogg");
	happy[1] = Load(".\\samples\\powerup\\happy1.ogg");
	happy[2] = Load(".\\samples\\powerup\\happy2.ogg");
	happy[3] = Load(".\\samples\\powerup\\happy3.ogg");
	happy[4] = Load(".\\samples\\powerup\\happy4.ogg");

	repair[0] = Load(".\\samples\\powerup\\repair0.ogg");
	repair[1] = Load(".\\samples\\powerup\\repair1.ogg");
	repair[2] = Load(".\\samples\\powerup\\repair2.ogg");

	reverse[0] = Load(".\\samples\\powerup\\reverse0.ogg");
	reverse[1] = Load(".\\samples\\powerup\\reverse1.ogg");
	reverse[2] = Load(".\\samples\\powerup\\reverse2.ogg");
	reverse[3] = Load(".\\samples\\powerup\\reverse3.ogg");
	reverse[4] = Load(".\\samples\\powerup\\reverse4.ogg");

	sensibility[0] = Load(".\\samples\\powerup\\sensibility0.ogg");
	sensibility[1] = Load(".\\samples\\powerup\\sensibility1.ogg");
	sensibility[2] = Load(".\\samples\\powerup\\sensibility2.ogg");
	sensibility[3] = Load(".\\samples\\powerup\\sensibility3.ogg");

	shield[0] = Load(".\\samples\\powerup\\shield0.ogg");
	shield[1] = Load(".\\samples\\powerup\\shield1.ogg");
	shield[2] = Load(".\\samples\\powerup\\shield2.ogg");
	shield[3] = Load(".\\samples\\powerup\\shield3.ogg");

	stop[0] = Load(".\\samples\\powerup\\stop0.ogg");
	stop[1] = Load(".\\samples\\powerup\\stop1.ogg");
	stop[2] = Load(".\\samples\\powerup\\stop2.ogg");
	stop[3] = Load(".\\samples\\powerup\\stop3.ogg");

	turbo[0] = Load(".\\samples\\powerup\\turbo0.ogg");
	turbo[1] = Load(".\\samples\\powerup\\turbo1.ogg");
	turbo[2] = Load(".\\samples\\powerup\\turbo2.ogg");
	turbo[3] = Load(".\\samples\\powerup\\turbo3.ogg");

	soundList[ACCEPTSOUND] = Load(".\\samples\\accept.wav");
	soundList[SELECTSOUND] = Load(".\\samples\\scroll.wav");
	soundList[BACKSOUND] = Load(".\\samples\\back.wav");
	soundList[QUITSOUND] = Load(".\\samples\\quit.wav");

	// Comment one of these two to check the other how it sounds
	soundList[ENGINELOOP] = Load(".\\samples\\car_engine_SuperLoop.wav");
	//soundlist[ENGINELOOP] = Load(".\\samples\\use.wav");
}


void Sound::Play(HSAMPLE &sound)
{
	Play(sound, 1.0f, 1.0f);
}

void Sound::Play(HSAMPLE &sound, float volume, float pitch)
{
	HCHANNEL channels = BASS_SampleGetChannel(sound, FALSE);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_VOL, volume);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_PAN, 0);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_FREQ, pitch);

	if (!BASS_ChannelPlay(channels, FALSE))
		std::cerr << "Can`t play sample \n";
}

void Sound::Play(HSAMPLE &sound, float volume)
{
	Play(sound, volume, 1.0f);
}

void Sound::Pause(HSAMPLE &sound)
{
	BASS_SampleStop(sound);
}

void Sound::PauseMusic()
{
	BASS_SampleStop(currentMusic);
}

void Sound::Resume()
{
	BASS_Start();
}

HSAMPLE Sound::Load(char *filename)
{
	HSAMPLE sample;
	sample = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_OVER_POS);

	if (sample) return sample;
	else
	{
		std::cerr << "Error loading sample " << filename << "\n";
		return -1;
	}
}