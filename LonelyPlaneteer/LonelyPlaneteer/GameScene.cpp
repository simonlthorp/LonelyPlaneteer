#include "Game.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "MapManager.h"
#include "Physics.h"

#include "ECS.h"
#include "Component.h"
#include "Player.h"
#include "Wall.h"
#include "Vector2D.h"
#include "Camera.h"
#include "Enemy.h"
#include "Item.h"
#include "Ship.h"
#include "ShipWall.h"
#include "GameOver.h"

#include "WinGame.h"

#include "EnemyManager.h"

//#include "UIManager.h"

Manager GameScene::wallManager;
Manager GameScene::entityManager;
Manager GameScene::shipManager;
bool GameScene::shipBuilt;
//Non-Static
MapManager *mapmanager;

//Audio
AudioMixer* GameScene::mixer;

Ship* ship = nullptr;
Player *player = nullptr;
Item *gearItem = nullptr;
Item *crystalItem = nullptr;
Item *turretItem = nullptr;
Enemy *enemy = nullptr;
GameOver *gameOver = nullptr;
WinGame* youWin = nullptr;
//UIManager* uiManager = nullptr;
EnemyManager *enemyManager = nullptr;

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::init() {
	mapmanager = new MapManager();

	SDL_Rect center = mapmanager->getCenterPoint();
	player = new Player(center.y - 300, center.x + 300);

	gearItem = new Item(100, 100, 1, "Assets/Gear.png");
	crystalItem = new Item(100, 3000, 2, "Assets/crystal_smaller.png");
	turretItem = new Item(3000, 100, 3, "Assets/turret_item.png");

	ship = new Ship(center.y, center.x);

	const int enemyCount = 32;
	enemyManager = new EnemyManager(player, mapmanager, mapmanager->getMap(), enemyCount);

	int windowX;
	int windowY;
	SDL_GetWindowSize(Game::window, &windowX, &windowY);
	gameOver = new GameOver(windowX/2 - 275.0f, windowY/2 - 300.0f);
	youWin = new WinGame(windowX / 2 - 220.0f, windowY / 2 - 300.0f);

	GameScene::shipManager.addNewEntity(ship);
	//GameScene::entityManager.addNewEntity(enemy);
	GameScene::entityManager.addNewEntity(gearItem);
	GameScene::entityManager.addNewEntity(crystalItem);
	GameScene::entityManager.addNewEntity(turretItem);
	GameScene::entityManager.addNewEntity(player);
	GameScene::entityManager.addNewEntity(gameOver);
	GameScene::entityManager.addNewEntity(youWin);

	mapmanager->drawAllColiders();

	//Audio
	mixer = new AudioMixer();
	//mixer->init();
	shipBuilt = false;
}

void GameScene::handleEvents() {
	player->eventHandle();
}

void GameScene::update() {
	GameScene::entityManager.refresh();
	GameScene::wallManager.refresh();
	GameScene::shipManager.refresh();

	GameScene::wallManager.update();
	GameScene::entityManager.update();
	GameScene::shipManager.update();

	enemyManager->update();

	Game::camera.x = player->getComponent <TransformComponent>().originX - (Game::camera.w / 2);
	Game::camera.y = player->getComponent <TransformComponent>().originY - (Game::camera.h / 2);
	
	
	//Game Over
	if (player->getComponent<HealthComponent>().getHealth() <= 0) {
	//	std::cout << "Game Over" << std::endl;
	//	//SDL_Rect center = mapmanager->getCenterPoint();
		gameOver->setGameOver();
	}

	if (shipBuilt) {
		//mixer->playWinMusic();
		youWin->setWin();
	}
	
}

void GameScene::render() {
	mapmanager->drawMap(Game::camera);
	GameScene::wallManager.draw(Game::camera);
	GameScene::shipManager.draw(Game::camera);
	player->draw(Game::camera);
	entityManager.draw(Game::camera);
	//player->uiManager->healthUI->draw();   // draw health bar
}