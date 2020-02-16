#pragma once
#include "Game.h"
#include "ECS.h"
#include "Component.h"
class Input : public Component {
public:
	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;
	bool keyE = false;

	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	void update() override {

		if (!keystates[SDL_SCANCODE_W])
		{
			keyW = false;
		}

		if (!keystates[SDL_SCANCODE_S]) {
			keyS = false;
		}

		if (!keystates[SDL_SCANCODE_A])
		{
			keyA = false;
		}

		if (!keystates[SDL_SCANCODE_D]) {
			keyD = false;
		}

		if (!keystates[SDL_SCANCODE_E]) {
			keyE = false;
		}

		//--------------------------------------------------------------

		if (keystates[SDL_SCANCODE_W])
		{
			keyW = true;
		}

		if (keystates[SDL_SCANCODE_A])
		{
			keyA = true;
		}

		if (keystates[SDL_SCANCODE_D])
		{
			keyD = true;
		}

		if (keystates[SDL_SCANCODE_S])
		{
			keyS = true;
		}

		if (keystates[SDL_SCANCODE_E]) {
			keyE = true;
		}
	}
};