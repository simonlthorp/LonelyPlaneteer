#pragma once
#ifndef LoadingScene_h
#define LoadingScene_h

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <iostream>

class LoadingScene {
public:
	LoadingScene();
	~LoadingScene();

	void init();
	void handleEvents();
	void update();
	void render();
private:
	void incrementCounter();

	SDL_Rect srcCircle, destCircle;
	SDL_Rect srcBackground, destBackground;
	SDL_Texture *circle1, *circle2, *circle3, *circle4, *circle5, *circle6, *circle7, *circle8, *circle9, *circle10, *circle11, *background;
	int circle1Counter = 0, circle2Counter = 0, circle3Counter = 0, circle4Counter = 0, circle5Counter = 0, circle6Counter = 0;
};
#endif