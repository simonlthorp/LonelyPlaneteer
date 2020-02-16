#pragma once

#include "SDL_mixer.h"
#include "SoundFX.h"

class AudioMixer
{
public:
	Mix_Music* music;
	Mix_Music* winMusic;
	Mix_Music* loseMusic;
	SoundFX* laserShot;
	SoundFX* ricochet;
	SoundFX* ricochet_metal;

	AudioMixer();
	~AudioMixer();

	void init();

	void playWinMusic();

private:
	int const FREQUENCY = 44100;
	int const CHUNK_SIZE = 2048;
	

};
