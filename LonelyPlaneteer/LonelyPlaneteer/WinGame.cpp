#include "WinGame.h"

WinGame::WinGame(float x, float y) {

	//440 x 280
	this->addComponent<TransformComponent>(x, y, 280, 440, 1);
	this->addComponent<SpriteComponent>("Assets/you_win.png");
	this->addComponent<ColliderComponent>("Win");

	float destX = this->getComponent<TransformComponent>().position.x;
	float destY = this->getComponent<TransformComponent>().position.y;
	int sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(x, y, sc);

}

void WinGame::update() {

}

void WinGame::draw(SDL_Rect cam) {

	if (win) {
		SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
		SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
		SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
		TextureManager::Draw(tex, src, dest, 0);
	}

}