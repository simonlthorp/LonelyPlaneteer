#pragma once
#ifndef SceneManager_h
#define SceneManager_h

#include "SDL.h"
#include "SDL_image.h"
#include "ECS.h"
#include "GameScene.h"
#include "LoadingScene.h"
#include "MenuScene.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	GameScene gameScene;
	static MenuScene menuScene;
	static LoadingScene loadScene;
	void init();
	void handleEvents();
	void update();
	void render();

	static void incrementLoadGame();
	static void decrementLoadGame();
	//DANGER WHEN SETTING THIS!
	static void setLoadGame(int value);

	static void incrementThreadCount();
	static void decrementThreadCount();
	//DANGER WHEN SETTING THIS!
	static void setThreadCount(int value);
	static void checkThreadCount();

	static int loadGame;
	static int threadCount;
	static std::vector<std::thread> threadList;
private:
	static std::mutex variable_mutex;
};
#endif
