#include "SceneManager.h"

#include <thread>
#include <vector>
#include <mutex>
#include <iostream>
#include <Windows.h>

//GameScene SceneManager::gameScene;
LoadingScene SceneManager::loadScene;
MenuScene SceneManager::menuScene;

int SceneManager::loadGame;
int SceneManager::threadCount;
std::vector<std::thread> SceneManager::threadList;
std::mutex SceneManager::variable_mutex;

SceneManager::SceneManager() {
	loadGame = 0;
	threadCount = 0;
}

SceneManager::~SceneManager() {}

void SceneManager::init() {
	loadGame = 0;
	menuScene.init();
	loadScene.init();
	gameScene.init();
}

void SceneManager::handleEvents() {
	switch (loadGame) {
	case 0:
		menuScene.handleEvents();
		break;
	case 1:
		//Calling the load handleEvents Here
		//For threadList.size() to work with threadCount it must be done serialized
		//threadCounter equals total number of running threads... no threads running equals finished work.
		if (threadCount == 0) {
			std::cout << "Finished Loading Game" << std::endl;
			incrementLoadGame();
		}
		break;
	case 2:
		gameScene.handleEvents();
		break;
	default:
		std::cerr << "Error Default HandleEvents Switch was called" << std::endl;
		break;
	}
}

void SceneManager::update() {
	switch (loadGame) {
	case 0:
		//Calling the menu update Here
		break;
	case 1:
		loadScene.update();
		break;
	case 2:
		gameScene.update();
		//Calling the game update Here
		break;
	default:
		std::cerr << "Error Default Update Switch was called" << std::endl;
		break;
	}
}

void SceneManager::render() {
	switch (loadGame) {
	case 0:
		//for (int i = 0; i < threadList.size(); i++) {
		//	if (threadList.at(i).joinable()) {
		//		std::cout << "Joining thread: " << i << std::endl;
		//		threadList.at(i).join();
		//	}
		//}
		menuScene.render();
		break;
	case 1:
		//for (int i = 0; i < threadList.size(); i++) {
		//	if (threadList.at(i).joinable()) {
		//		std::cout << "Joining thread: " << i << std::endl;
		//		threadList.at(i).join();
		//	}
		//}
		loadScene.render();
		break;
	case 2:
		gameScene.render();
		break;
	default:
		std::cerr << "Error Default Render Switch was called" << std::endl;
		break;
	}
}


//----------------------------------
// Used by parallel threads when running.
// See MapManager for examples

void SceneManager::incrementLoadGame() {
	variable_mutex.lock();
	loadGame++;
	variable_mutex.unlock();
}

void SceneManager::decrementLoadGame() {
	variable_mutex.lock();
	loadGame--;
	variable_mutex.unlock();
}

//DANGER WHEN SETTING THIS!
void SceneManager::setLoadGame(int value) {
	variable_mutex.lock();
	loadGame = value;
	variable_mutex.unlock();
}

void SceneManager::incrementThreadCount() {
	variable_mutex.lock();
	threadCount++;
	variable_mutex.unlock();
}

void SceneManager::decrementThreadCount() {
	variable_mutex.lock();
	threadCount--;
	variable_mutex.unlock();
}

//DANGER WHEN SETTING THIS!
void SceneManager::setThreadCount(int value) {
	variable_mutex.lock();
	threadCount = value;
	variable_mutex.unlock();
}

void SceneManager::checkThreadCount() {
	variable_mutex.lock();
	std::cout << "Thread Count: (" << threadList.size() << "/" << (threadList.size() - threadCount) << ")" << std::endl;
	variable_mutex.unlock();
}