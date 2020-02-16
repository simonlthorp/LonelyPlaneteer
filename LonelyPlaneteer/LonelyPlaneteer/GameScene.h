#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "ECS.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "AudioMixer.h"

class GameScene {
public:
	static Manager entityManager;
	static Manager wallManager;
	static Manager shipManager;
	static bool shipBuilt;
	static AudioMixer* mixer;
	GameScene();
	~GameScene();

	void init();

	void handleEvents();
	void update();
	void render();
private:
	

};