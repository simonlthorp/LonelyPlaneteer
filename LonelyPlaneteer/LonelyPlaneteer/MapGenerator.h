#pragma once

#include "MapData.h"
#include <stdio.h>
#include <iostream>
#include "fstream"
#include "sstream"
#include <math.h>
#include <chrono>
#include <cstdlib>
#include <ctime>

class MapGenerator {
public:
	MapGenerator();
	~MapGenerator();
	static MapData createMap(int width, int height, int scale, int tunnelSize, int tunnelLength); //Recreates the map using the current settings
	//static MapData recreateMap(int width, int height, int scale, int tunnelSize, int tunnelLength); //Recreates the map with new settings
private:
	static int** establishMapScale(int** map, int width, int height, int scale);
};