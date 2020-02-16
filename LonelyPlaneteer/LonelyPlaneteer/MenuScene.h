#pragma once
#ifndef MenuScene_h
#define MenuScene_h

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "MenuSceneUI.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>

class MenuScene {
public:
	MenuScene();
	~MenuScene();

	void init();
	void handleEvents();
	void update();
	void render();

	//Mouse
	void updateMouse();
	int getMouseX();
	int getMouseY();
private:
	SDL_Rect src, dest;
	std::vector<MenuSceneUI> uiComponents;

	//Title
	SDL_Rect srcTitle, destTitle;
	SDL_Texture *title;

	//Mouse
	int mouse_x;
	int mouse_y;
};
#endif