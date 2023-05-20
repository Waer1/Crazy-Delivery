#include "sound.hpp"
#include <bass.h>
#include <iostream>

bool Sound::isInit = false;

Sound::Sound(const char *path, bool loop)
{
	if (!isInit)
	{
		if (!BASS_Init(-1, 44100, 0, 0, NULL))
		{
			std::cout << "Can't initialize device" << std::endl;
		}
		else
		{
			isInit = true;
		}
	}
	channel = BASS_StreamCreateFile(false, path, 0, 0, loop ? BASS_SAMPLE_LOOP : BASS_SAMPLE_FLOAT);
	if (!channel)
	{
		std::cout << "Can't load the sound" << std::endl;
	}
}
void Sound::changeVolume(int volume)
{

	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume * 100);
}
void Sound::play()
{

	BASS_ChannelPlay(channel, 0);
}
void Sound::pause()
{
	BASS_ChannelPause(channel);
}
void Sound::stop()
{
	BASS_ChannelStop(channel);
}
Sound::~Sound()
{
	BASS_ChannelStop(channel);
}