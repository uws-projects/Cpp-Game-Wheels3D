#include "Sound.h"

Sound * Sound::s_pInstance = 0;
HSAMPLE Sound::soundlist[NUMBEROFSOUNDS] = {};

void Sound::Initialize()
{
	soundlist[INTROSOUND] = Load(".\\samples\\use.wav");
}

void Sound::Play(HSAMPLE &sound)
{
	HCHANNEL channels = BASS_SampleGetChannel(sound, FALSE);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_FREQ, -1);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_VOL, 50);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_PAN, (rand()%201) - 100);

	if (!BASS_ChannelPlay(channels, FALSE))
		std::cerr << "Can`t play sample \n";
}

void Sound::Play(HSAMPLE &sound, float volume)
{
	HCHANNEL channels = BASS_SampleGetChannel(sound, FALSE);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_FREQ, -1);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_VOL, volume);
	BASS_ChannelSetAttribute(channels, BASS_ATTRIB_PAN, (rand() % 201) - 100);

	if (!BASS_ChannelPlay(channels, FALSE))
		std::cerr << "Can`t play sample \n";
}

void Sound::Pause(HSAMPLE &sound)
{
	BASS_SampleStop(sound);
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