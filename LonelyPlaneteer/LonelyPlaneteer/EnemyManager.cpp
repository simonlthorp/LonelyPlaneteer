#include "EnemyManager.h"


EnemyManager::EnemyManager(Player* player, MapManager * mm, const MapData& data, int count) : plyr(player) {
	mapWidth = data.width * data.scale;
	mapHeight = data.height * data.scale;

	pathfinder = new Pathfinder(data);

	int centerHeight = mapHeight / 2;
	int centerWidth = mapWidth / 2;

	const int spawnPointsCount = 4;

	int a = static_cast<int>(100 / 32);
	int b = static_cast<int>(2750 / 32);

	const SDL_Point topLeft = { a, a };
	const SDL_Point topRight = { b, a };
	const SDL_Point BottomLeft =  { a, b };
	const SDL_Point BottomRight = { b, b };
	SDL_Point spawnPoints[spawnPointsCount] = { topLeft, topRight, BottomLeft, BottomRight };

	int spawnCount = static_cast<int>(count / 4);

	int divisor = 2;

	int spawnCountX = static_cast<int>(spawnCount / divisor);
	int spawnCountY = divisor;

	int cnt = 0;

	for (int i = 0; i < spawnPointsCount; i++) {
		SDL_Point spawnPoint = spawnPoints[i];
		for (int y = 0; y < spawnCountY; y++) {
			for (int x = 0; x < spawnCountX; x++) {

				int spawnX = spawnPoint.x + x;
				int spawnY = spawnPoint.y + y;

				if (spawnX <= -1 || spawnX >= mapWidth ||
					spawnY <= -1 || spawnY >= mapHeight) {
					continue;
				}

				if (data.map[spawnX][spawnY] == 1) {
					continue;
				}

				spawnX *= 32;
				spawnY *= 32;

				Enemy* enemy = new Enemy(spawnX, spawnY, player, mm);
				enemies.push_back(enemy);
				GameScene::entityManager.addNewEntity(enemy);
				cnt++;
			}
		}
	}

	SDL_Rect center;
	center.x = centerWidth * 32;
	center.y = centerHeight * 32;

	Enemy* enemy1 = new Enemy(center.y - 500, center.x + 550, player, mm);
	Enemy* enemy2 = new Enemy(center.y - 500, center.x + 600, player, mm);
	Enemy* enemy3 = new Enemy(center.y - 500, center.x + 650, player, mm);
	Enemy* enemy4 = new Enemy(center.y - 500, center.x + 700, player, mm);
	Enemy* enemy5 = new Enemy(center.y - 500, center.x + 750, player, mm);
	Enemy* enemy6 = new Enemy(center.y - 500, center.x + 800, player, mm);
	Enemy* enemy7 = new Enemy(center.y - 500, center.x + 850, player, mm);
	Enemy* enemy8 = new Enemy(center.y - 500, center.x + 900, player, mm);
	Enemy* enemy9 = new Enemy(center.y - 500, center.x + 950, player, mm);
	Enemy* enemy10 = new Enemy(center.y - 500, center.x + 1000, player, mm);

	GameScene::entityManager.addNewEntity(enemy1);
	GameScene::entityManager.addNewEntity(enemy2);
	GameScene::entityManager.addNewEntity(enemy3);
	GameScene::entityManager.addNewEntity(enemy4);
	GameScene::entityManager.addNewEntity(enemy5);
	GameScene::entityManager.addNewEntity(enemy6);
	GameScene::entityManager.addNewEntity(enemy7);
	GameScene::entityManager.addNewEntity(enemy8);
	GameScene::entityManager.addNewEntity(enemy9);
	GameScene::entityManager.addNewEntity(enemy10);
}

EnemyManager::~EnemyManager() {
	enemies.clear();
}

void EnemyManager::init() {
}

void EnemyManager::handleEvents() {
}

void EnemyManager::update() {
	pathfinder->update(plyr->getComponent <TransformComponent>().position, enemies);
}

void EnemyManager::render() {
}
