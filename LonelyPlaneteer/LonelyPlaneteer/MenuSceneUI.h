#pragma once
#ifndef MenuSceneUI_h
#define MenuSceneUI_h

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <iostream>

class MenuSceneUI {
public:
	MenuSceneUI();
	MenuSceneUI(std::string id, std::string path, int width, int height, float x, float y, int pixX, int pixY);
	~MenuSceneUI();

	SDL_Rect getSrc();
	SDL_Rect getDest();
	SDL_Texture* getTexture();
	std::string getID();

	bool withinUI(int x, int y);
private:
	std::string id;
	SDL_Rect src, dest;
	SDL_Texture *texture;
	bool state = false;
};



#endif