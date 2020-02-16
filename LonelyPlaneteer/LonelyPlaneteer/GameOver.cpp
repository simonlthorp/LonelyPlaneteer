#include "GameOver.h"

GameOver::GameOver(float x, float y) {

	//550 x 420
	this->addComponent<TransformComponent>(x, y, 550, 420, 1);
	this->addComponent<SpriteComponent>("Assets/game_over.png");
	this->addComponent<ColliderComponent>("GameOver");

	float destX = this->getComponent<TransformComponent>().position.x;
	float destY = this->getComponent<TransformComponent>().position.y;
	int sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(x, y, sc);

}

void GameOver::update() {

}

void GameOver::draw(SDL_Rect cam) {
	
	if (over) {
		SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
		SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
		SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
		TextureManager::Draw(tex, src, dest, 0);
	}

}