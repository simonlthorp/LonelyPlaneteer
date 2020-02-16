#pragma once
#include "Game.h"
#include "MapData.h"
#include "MapGenerator.h"
#include <mutex>;
#include <thread>
#include <iostream>
#include <Windows.h>

class MapManager {
public:
	MapManager();
	~MapManager();

	int getMapWidth();
	int getMapHeight();
	int getMapScale();
	int getTunnelLength();
	int getTunnelSize();
	MapData getMap();
	int** getMapInfo();

	void loadMap();
	void createStartArea();
	bool withinMapAndNotPath(int** tmpMap, int height, int width);
	SDL_Rect getCenterPoint();
	void drawAllColiders();
	void drawCollisions();
	void drawOuterCollider();
	void drawMap(SDL_Rect cam);
private:
	SDL_Rect src, dest;
	SDL_Texture *dirt;
	SDL_Texture *water;
	SDL_Texture *grass;
	MapData map;

	//Threading
	std::mutex entity_mtx;
	int threadCounter = 0;

	void threadInnerCollider(int id, int starti, int endi, int startj, int endj);
	void threadWallCollider(int id, bool horizontal, int straitValue);
};