#pragma once

#include "MapManager.h"
#include "Vector2D.h"
#include "CL/cl.hpp"
#include "Enemy.h"
#include "OpenCLManager.h"
#include "PathNode.h"

class Pathfinder {
public:
	Pathfinder(MapData data);
	~Pathfinder();

	void init(MapData data);
	void update(Vector2D source, std::vector<Enemy*>& enemies);
	void draw();

private:
	OpenCLManager* openCLManager;

	cl_int* map;

	cl_int mapWidth;
	cl_int mapHeight;
	cl_int numOfTiles;

	unsigned int frameCount;
};