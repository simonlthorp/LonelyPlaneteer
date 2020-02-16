#include "LoadingScene.h"
#include "TextureManager.h"

LoadingScene::LoadingScene() {}
LoadingScene::~LoadingScene() {}

void LoadingScene::init() {
	circle1 = TextureManager::LoadTexture("Assets/Loading/circle1.png");
	circle2 = TextureManager::LoadTexture("Assets/Loading/circle2.png");
	circle3 = TextureManager::LoadTexture("Assets/Loading/circle3.png");
	circle4 = TextureManager::LoadTexture("Assets/Loading/circle4.png");
	circle5 = TextureManager::LoadTexture("Assets/Loading/circle5.png");
	circle6 = TextureManager::LoadTexture("Assets/Loading/circle6.png");
	circle7 = TextureManager::LoadTexture("Assets/Loading/circle7.png");
	circle8 = TextureManager::LoadTexture("Assets/Loading/circle8.png");
	circle9 = TextureManager::LoadTexture("Assets/Loading/circle9.png");
	circle10 = TextureManager::LoadTexture("Assets/Loading/circle10.png");
	circle11 = TextureManager::LoadTexture("Assets/Loading/circle11.png");

	destCircle.w = Game::camera.w / 2;
	destCircle.h = Game::camera.w / 2;
	destCircle.x = (Game::camera.w / 2) - (destCircle.w / 2);
	destCircle.y = (Game::camera.h / 2) - (destCircle.h / 2);

	srcCircle.w = 894;
	srcCircle.h = 894;
	srcCircle.x = 0;
	srcCircle.y = 0;

	background = TextureManager::LoadTexture("Assets/Loading/Background.png");
	destBackground.w = Game::camera.w;
	destBackground.h = Game::camera.h;
	destBackground.x = (Game::camera.w / 2) - (destBackground.w / 2);
	destBackground.y = (Game::camera.h / 2) - (destBackground.h / 2);

	srcBackground.w = 663;
	srcBackground.h = 492;
	srcBackground.x = 0;
	srcBackground.y = 0;
}

void LoadingScene::handleEvents() {}

void LoadingScene::update() {
	incrementCounter();
}

void LoadingScene::render() {
	TextureManager::Draw(background, srcBackground, destBackground, Game::camera, 0);

	TextureManager::Draw(circle1, srcCircle, destCircle, Game::camera, circle1Counter);
	TextureManager::Draw(circle2, srcCircle, destCircle, Game::camera, circle2Counter);
	TextureManager::Draw(circle3, srcCircle, destCircle, Game::camera, circle3Counter);
	TextureManager::Draw(circle4, srcCircle, destCircle, Game::camera, circle4Counter);
	TextureManager::Draw(circle5, srcCircle, destCircle, Game::camera, circle5Counter);
	TextureManager::Draw(circle6, srcCircle, destCircle, Game::camera, circle6Counter);
	TextureManager::Draw(circle7, srcCircle, destCircle, Game::camera, circle1Counter);
	TextureManager::Draw(circle8, srcCircle, destCircle, Game::camera, circle2Counter);
	TextureManager::Draw(circle9, srcCircle, destCircle, Game::camera, circle3Counter);
	TextureManager::Draw(circle10, srcCircle, destCircle, Game::camera, circle4Counter);
	TextureManager::Draw(circle11, srcCircle, destCircle, Game::camera, circle5Counter);
}

void LoadingScene::incrementCounter() {
	circle1Counter += 1;
	if (circle1Counter > 360) {
		circle1Counter = 0;
	}

	circle2Counter += 2;
	if (circle2Counter > 360) {
		circle2Counter = 0;
	}

	circle3Counter += 3;
	if (circle3Counter > 360) {
		circle3Counter = 0;
	}

	circle4Counter += 4;
	if (circle4Counter > 360) {
		circle4Counter = 0;
	}

	circle5Counter += 5;
	if (circle5Counter > 360) {
		circle5Counter = 0;
	}

	circle6Counter += 6;
	if (circle6Counter > 360) {
		circle6Counter = 0;
	}
}