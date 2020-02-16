#include "MapGenerator.h"
#include "GameScene.h"

MapGenerator::MapGenerator() {}
MapGenerator::~MapGenerator() {}

MapData MapGenerator::createMap(int width, int height, int scale, int tunnelSize, int tunnelLength) {
	const long double sysTime = time(0);
	const long double sysTimeMS = sysTime * 1000;
	srand(sysTime);

	int** tmp;
	tmp = new int*[height];
	for (int i = 0; i < height; i++) {
		tmp[i] = new int[width] {1};
		for (int j = 0; j < width; j++) {
			tmp[i][j] = 1;
		}
	}

	int max_tunnels = tunnelSize;

	int currentRow = (rand() % height);
	int currentColumn = (rand() % width);

	int directions[4][2] = { {-1,0}, {1,0}, {0, -1}, {0,1} };
	int lastDirection[2] = { 0,0 };
	int randomDirection[2] = { 0,0 };

	while (max_tunnels && tunnelLength) {
		do {
			int rd = floor((rand() % (sizeof(directions) / sizeof(*directions))));
			randomDirection[0] = directions[rd][0];
			randomDirection[1] = directions[rd][1];
		} while (
			(randomDirection[0] == -lastDirection[0] && randomDirection[1] == -lastDirection[1]) ||
			(randomDirection[0] == lastDirection[0] && randomDirection[1] == lastDirection[1])
			);

		int randomLength = ceil((rand() % tunnelLength) + 1);
		int tunnelLength = 0;

		while (tunnelLength < randomLength) {
			if (
				((currentRow == 0) && (randomDirection[0] == -1)) ||
				((currentColumn == 0) && (randomDirection[1] == -1)) ||
				((currentRow == height - 1) && (randomDirection[0] == 1)) ||
				((currentColumn == width - 1) && (randomDirection[1] == 1))
				) {
				break;
			}
			else {
				if (currentRow > height || currentColumn > width || currentRow < 0 || currentColumn < 0) {
					std::cout << "Error" << std::endl;
				}
				tmp[currentRow][currentColumn] = 0;
				currentRow += randomDirection[0];
				currentColumn += randomDirection[1];
				tunnelLength++;
			}
		}

		if (tunnelLength) {
			lastDirection[0] = randomDirection[0];
			lastDirection[1] = randomDirection[1];
			max_tunnels--;
		}
	}

	//map = tmp; // all our tunnels have been created and our map is complete, so lets return it to our render()
	
	if (scale < 2) {
		return MapData(tmp, width, height, scale, tunnelSize, tunnelLength);
	}
	else {
		return MapData(establishMapScale(tmp, width, height, scale), width, height, scale, tunnelSize, tunnelLength);
	}
}

//Privates
int** MapGenerator::establishMapScale(int** map, int width, int height, int scale) {
	if (scale < 2) {
		std::cout << "Scale is less than 2" << std::endl;
		//throw error!
	}

	int** tmp;
	tmp = new int*[height * scale];
	for (int i = 0; i < height * scale; i++) {
		tmp[i] = new int[width * scale]{ 1 };
		for (int j = 0; j < width * scale; j++) {
			tmp[i][j] = 2;
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < scale; k++) {
				for (int l = 0; l < scale; l++) {
					tmp[(i*scale) + k][(j*scale) + l] = map[i][j];
					int tmpx = (i + k);
					int tmpy = (j + l);
				}
			}
		}
	}

	//Print Map Here
	//for (int i = 0; i < height * scale; i++) {
	//	for (int j = 0; j < width * scale; j++) {
	//		std::cout << tmp[i][j] << ",";
	//	}
	//	std::cout << std::endl;
	//}

	return tmp;
}