#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "ECS.h"
#include "SceneManager.h"

#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>

class Game {
public:

	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() {return isRunning;};

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Window* window;
	static SDL_Rect camera;
	static SDL_Window* getWindow();
	static SceneManager sceneManager;
	static std::mutex rendering;
private:
	bool isRunning;

	
};