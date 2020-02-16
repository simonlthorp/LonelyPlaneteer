#pragma once

#include "SDL.h"
#include "SDL_mixer.h"

class SoundFX
{
public:
	Mix_Chunk* sfx;

	SoundFX(const char* filename);
	~SoundFX();

	void loadAudio(const char* filename);
	void play();

private:
	

};
