#pragma once

#include "SDL.h"
#include "MapData.h"
#include "Enemy.h"
#include "Pathfinder.h"
#include "MapManager.h"

static class EnemyManager {
public:
	EnemyManager(Player* player, MapManager *, const MapData& data, int count);
	~EnemyManager();

	void init();
	void handleEvents();
	void update();
	void render();

private:
	int mapWidth;
	int mapHeight;

	std::vector<Enemy*> enemies;

	Pathfinder *pathfinder;
	Player* plyr;
};

