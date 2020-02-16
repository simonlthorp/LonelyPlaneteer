#include "MapManager.h"
#include "TextureManager.h"
#include "Wall.h"
#include <vector>
#include <chrono>

MapManager::MapManager() {
	dirt = TextureManager::LoadTexture("Assets/Desert_Purple.png");
	grass = TextureManager::LoadTexture("Assets/Desert_Purple.png");
	water = TextureManager::LoadTexture("Assets/Desert_Purple.png");

	src.x = 0;
	src.y = 0;
	src.w = 32;
	src.h = 32;

	dest.w = 32;
	dest.h = 32;
	dest.x = 0;
	dest.y = 0;
	loadMap();
}

MapManager::~MapManager() {
	SDL_DestroyTexture(dirt);
	SDL_DestroyTexture(grass);
	SDL_DestroyTexture(water);
}

int MapManager::getMapWidth() { return map.width; }
int MapManager::getMapHeight() { return map.height; }
int MapManager::getMapScale() { return map.scale; }
int MapManager::getTunnelLength() { return map.tunnelLength; }
int MapManager::getTunnelSize() { return map.tunnelSize; }
MapData MapManager::getMap() { return map; }
int** MapManager::getMapInfo() { return map.map; }

void MapManager::loadMap() {
	map = MapGenerator().createMap(50, 50, 2, 510, 10);
	//map = MapGenerator().createMap(100, 100, 2, 2040, 10);
	createStartArea();
}


void MapManager::createStartArea() {
	int centerHeight = (map.height * map.scale) / 2;
	int centerWidth = (map.width * map.scale) / 2;
	int carvingAmount = 15; // THIS IS THE SIZE OF THE STARTING AREA (AKA. 20x20)
	
	//if ((centerHeight - carvingAmount) < 0 || (centerHeight + carvingAmount) > map.height || (centerWidth - carvingAmount) < 0 || (centerWidth + carvingAmount) > map.width) {
	//	std::cerr << "Failed to create starting area!";
	//}
	int** tmpMap = map.map;

	//Make Center Area
	for (int i = (centerHeight - carvingAmount); i < (centerHeight + carvingAmount); i++) {
		for (int j = (centerWidth - carvingAmount); j < (centerWidth + carvingAmount); j++) {
			tmpMap[i][j] = 0;
		}
	}


	int squareSize = 20;
	//Top Left
	std::cout << "0" << ":" << "0" << std::endl;
	for (int i = 0; i < squareSize; i++) {
		for (int j = 0; j < squareSize; j++) {
			tmpMap[i][j] = 0;
		}
	}

	//Top Right
	std::cout << "0" << ":" << (map.width * map.scale) - squareSize << std::endl;
	for (int i = 0; i < squareSize; i++) {
		for (int j = (map.width * map.scale) - 1; j > ((map.width * map.scale) - squareSize); j--) {
			tmpMap[i][j] = 0;
		}
	}

	//Bottom Left
	std::cout << (map.height * map.scale) - squareSize << ":" << "0" << std::endl;
	for (int i = (map.height * map.scale) - 1; i > ((map.height * map.scale) - squareSize); i--) {
		for (int j = 0; j < squareSize; j++) {
			tmpMap[i][j] = 0;
		}
	}

	//Bottom Right
	std::cout << (map.height * map.scale) - squareSize << ":" << (map.width * map.scale) - squareSize << std::endl;
	for (int i = (map.height * map.scale) - 1; i > ((map.height * map.scale) - squareSize); i--) {
		for (int j = (map.width * map.scale) - 1; j > ((map.width * map.scale) - squareSize); j--) {
			tmpMap[i][j] = 0;
		}
	}

	//Reduce Colliders
	for (int i = 0; i < (map.height * map.scale); i++) {
		for (int j = 0; j < (map.width * map.scale); j++) {
			if (tmpMap[i][j] == 0) {
				for (int k = (i - 1); k <= (i + 1); k++) {
					for (int m = (j - 1); m <= (j + 1); m++) {
						if (withinMapAndNotPath(tmpMap, k, m)) {
							tmpMap[k][m] = 2;
						}
					}
				}
			}
		}
	}

	////------------------------
	////Prints the created map
	////------------------------
/*	for (int i = 0; i < map.height * map.scale; i++) {
		for (int j = 0; j < map.width * map.scale; j++) {
			std::cout << tmpMap[i][j] << ",";
		}
		std::cout << std::endl;
	}*/

	map.map = tmpMap;
}

bool MapManager::withinMapAndNotPath(int** tmpMap, int height, int width) {
	if (height >= 0 && height < (map.height * map.scale) && width >= 0 && width < (map.width * map.scale)) {
		if (tmpMap[height][width] != 0 && tmpMap[height][width] != 2) {
			return true;
		}
	}
	return false;
}


SDL_Rect MapManager::getCenterPoint() {
	SDL_Rect center;
	int centerHeight = (map.height * map.scale) / 2;
	int centerWidth = (map.width * map.scale) / 2;
	center.x = centerWidth * 32;
	center.y = centerHeight * 32;
	return center;
}

void MapManager::drawAllColiders() {
	drawCollisions();
	drawOuterCollider();
	//SceneManager::threadCount = SceneManager::threadList.size();
	for (int i = 0; i < SceneManager::threadList.size(); i++) {
		SceneManager::threadList.at(i).detach();
	}
	//std::this_thread::sleep_for(std::chrono::seconds(30));
	std::cout << "Total number of threads: " << SceneManager::threadList.size() << std::endl;
}

void MapManager::drawCollisions() {
	int numberOfSection = 5; // 2 sections = 4 divisions.. 4 sections = 16 divisions.. aka. numberOfSection^2
	int heightSection = (map.height * map.scale) / numberOfSection;
	int widthSection = (map.width * map.scale) / numberOfSection;
	for (int i = 0; i < numberOfSection; i++) {
		for (int j = 0; j < numberOfSection; j++) {
			SceneManager::threadList.push_back(std::thread(&MapManager::threadInnerCollider, this, SceneManager::threadList.size() + 1, (heightSection * i), (heightSection * (i + 1)), (widthSection * j), (widthSection * (j + 1))));
		}
	}
}

void MapManager::drawOuterCollider() {
	Wall* tmpWall = nullptr;
	int topRow = -1; //starts at top row
	int bottomRow = (map.height * map.scale); //starts at bottom row
	int leftColumn = -1; //starts at left column
	int rightColumn = (map.width * map.scale); //starts at right column
	//Top Wall
	SceneManager::threadList.push_back(std::thread(&MapManager::threadWallCollider, this, SceneManager::threadList.size() + 1, true, topRow));
	//Bottom Wall
	SceneManager::threadList.push_back(std::thread(&MapManager::threadWallCollider, this, SceneManager::threadList.size() + 1, true, bottomRow));
	//Left Wall
	SceneManager::threadList.push_back(std::thread(&MapManager::threadWallCollider, this, SceneManager::threadList.size() + 1, false, leftColumn));
	//Right Wall
	SceneManager::threadList.push_back(std::thread(&MapManager::threadWallCollider, this, SceneManager::threadList.size() + 1, false, rightColumn));
}

void MapManager::threadInnerCollider(int id, int starti, int endi, int startj, int endj) {
	SceneManager::incrementThreadCount();
	std::chrono::milliseconds begining = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch() );
	int** mapInfo = map.map;
	SDL_Texture* texture = TextureManager::LoadTexture("Assets/Desert.png");
	for (int row = starti; row < endi; row++) {
		for (int col = startj; col < endj; col++) {
			//std::cout << "thread: " << row << ":" << col << std::endl;
			entity_mtx.lock();
			if (mapInfo[row][col] == 2) {
				Game::sceneManager.gameScene.wallManager.addNewEntity(new Wall(texture, col * 32, row * 32));
			}
			entity_mtx.unlock();
		}
	}

	std::chrono::milliseconds ending = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds total = ending - begining;
	std::cout << "Thread Finished " << id << " sections: " << starti << "," << endi << " : " << startj << "," << endi << " in: " << (total.count()) << " (milliseconds)" << std::endl;
	SceneManager::decrementThreadCount();
	SceneManager::checkThreadCount();
}

void MapManager::threadWallCollider(int id, bool horizontal, int straitValue) {
	SceneManager::incrementThreadCount();
	SDL_Texture* texture = TextureManager::LoadTexture("Assets/Desert.png");
	if (horizontal) {
		for (int i = -1; i < (map.width * map.scale) + 1; i++) {
			entity_mtx.lock();
			GameScene::wallManager.addNewEntity(new Wall(texture, i * 32, straitValue * 32));
			entity_mtx.unlock();
			//std::cout << topRow << " " << i << " : ";
		}
	}
	else if (!horizontal) {
		for (int i = -1; i < (map.height * map.scale) + 1; i++) {
			entity_mtx.lock();
			GameScene::wallManager.addNewEntity(new Wall(texture, straitValue * 32, i * 32));
			entity_mtx.unlock();
			//std::cout << i << " " << leftColumn << " : ";
		}
	}

	std::cout << "Wall Thread " << id << " Finished: " << horizontal << ": " << straitValue << std::endl;
	SceneManager::decrementThreadCount();
	SceneManager::checkThreadCount();
}

void MapManager::drawMap(SDL_Rect cam) {
	int **mapInfo = map.map;
	int type = 0;

	for (int row = 0; row < (map.height * map.scale); row++) {
		for (int col = 0; col < (map.width * map.scale); col++) {
			type = mapInfo[row][col];
			dest.x = col * 32;
			dest.y = row * 32;
			switch (type) {
			case 0:
				src.x = 100;
				src.y = 0;
				TextureManager::Draw(water, src, dest, cam, 0);
				break;
			case 1:
				src.x = 25;
				src.y = 90;
				TextureManager::Draw(dirt, src, dest, cam, 0);
				break;
			//case 2:
			//	src.x = 25;
			//	src.y = 80;
			//	TextureManager::Draw(grass, src, dest, cam, 0);
			//	break;
			default:
				break;
			}
		}
	}
}