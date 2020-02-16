#include "Pathfinder.h"
#include <chrono>

Pathfinder::Pathfinder(MapData data) {
	// save calculated map size to config
	mapWidth = data.width * data.scale;
	mapHeight = data.height * data.scale;
	numOfTiles = mapWidth * mapHeight;

	init(data);

	openCLManager = new OpenCLManager(map, numOfTiles);
}

Pathfinder::~Pathfinder()
{
	free(map);
}

void Pathfinder::init(MapData data) {
	frameCount = 0;

	map = (cl_int*) std::malloc(sizeof(cl_int) * numOfTiles);

	// convert 2d map data to 1d, so we can work with it in OpenCL kernels
	for (int j = 0; j < mapHeight; j++) {
		for (int i = 0; i < mapWidth; i++) {
			map[j * mapWidth + i] = (cl_int) data.map[i][j];
		}
	}
}

void Pathfinder::update(Vector2D source, std::vector<Enemy*>& enemies) {
	//auto start = std::chrono::steady_clock::now();

	// convert world (pixel) coords to local (tile index) coords
	cl_uint sourceX = (cl_uint)source.x / 32;
	cl_uint sourceY = (cl_uint)source.y / 32;

	cl_uint enemyCount = enemies.size();

	cl_uint* targetX = (cl_uint*) malloc(sizeof(cl_uint) * enemyCount);
	cl_uint* targetY = (cl_uint*) malloc(sizeof(cl_uint) * enemyCount);

	cl_int* pathX = (cl_int*)malloc(sizeof(cl_int) * enemyCount);
	cl_int* pathY = (cl_int*)malloc(sizeof(cl_int) * enemyCount);

	int i = 0;

	for (Enemy*& e : enemies) {
		if (!e->getIsDead()) {
			TransformComponent eTransform = e->getComponent<TransformComponent>();
			targetX[i] = (cl_uint)eTransform.position.x / 32;
			targetY[i] = (cl_uint)eTransform.position.y / 32;
		}
		i++;
	}

	bool result = openCLManager->runAStar(targetX, targetY, enemyCount, sourceX, sourceY, pathX, pathY);

	i = 0;
	for (Enemy*& e : enemies) {
		if (!e->getIsDead()) {
			e->setPath(pathX[i], pathY[i]);
		}
		i++;
	}

	//auto end = std::chrono::steady_clock::now();

	// Store the time difference between start and end
	//auto diff = end - start;

	//std::cout << "Found " << enemyCount << " paths in " << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;

	free(targetX);
	free(targetY);
	free(pathX);
	free(pathY);
}

void Pathfinder::draw()
{

}
