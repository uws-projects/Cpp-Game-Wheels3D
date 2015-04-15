#include "Sound.h"

Sound * Sound::s_pInstance = 0;
HSAMPLE Sound::soundlist[NUMBEROFSOUNDS] = {};

void Sound::Initialize()
{
	soundlist[ACCEPTSOUND] = Load(".\\samples\\accept.wav");
	soundlist[SELECTSOUND] = Load(".\\samples\\scroll.wav");
	soundlist[BACKSOUND] = Load(".\\samples\\back.wav");
	soundlist[MUSICTITLE] = Load(".\\music\\music01.ogg");
	soundlist[MUSICCREDITS] = Load(".\\music\\music02.ogg");
	soundlist[MUSICSCOREBOARD] = Load(".\\music\\music03.ogg");
	soundlist[MUSICSETUP] = Load(".\\music\\music04.ogg");
	soundlist[MUSICMENU] = Load(".\\music\\music05.ogg");
	soundlist[QUITSOUND] = Load(".\\samples\\quit.wav");

	// Comment one of these two to check the other how it sounds
	soundlist[ENGINELOOP] = Load(".\\samples\\car_engine_SuperLoop.wav");
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

void Sound::Pause(int sound)
{
	BASS_SampleStop(soundlist[sound]);
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