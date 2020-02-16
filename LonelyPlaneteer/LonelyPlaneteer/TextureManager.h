#pragma once

#include "Game.h"
#include <mutex>

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* filename);
	static void Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, double degrees);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_Rect cam, double degrees);
	static void DrawRect(SDL_Rect *rect);

};
