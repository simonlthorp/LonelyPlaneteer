#pragma once

struct MapData {
	MapData() { /* Warning this will cause errors if data members are not set after */ }
	MapData(int** map, int width, int height, int scale, int tunnelSize, int tunnelLength) : map(map), width(width), height(height), scale(scale), tunnelSize(tunnelSize), tunnelLength(tunnelLength) {}
	int **map;
	int width;
	int height;
	int scale;
	int tunnelSize;
	int tunnelLength;
};
