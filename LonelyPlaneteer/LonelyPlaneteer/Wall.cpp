#include "Wall.h"

Wall::Wall(SDL_Texture* texture, float x, float y) {
	int sc;
	float destX, destY;
	//Add components
	//std::cout << "Adding Transform" << std::endl;
	this->addComponent<TransformComponent>(x, y, 32, 32, 1);
	//std::cout << "Adding Sprite" << std::endl;
	//this->addComponent<SpriteComponent>("Assets/Desert.png");
	this->addComponent<SpriteComponent>(texture);
	//std::cout << "Adding Collider" << std::endl;
	this->addComponent<ColliderComponent>("Wall");

	//std::cout << "Updating Transform" << std::endl;
	//set destination Rect for Sprite
	destX = this->getComponent<TransformComponent>().position.x;
	destY = this->getComponent<TransformComponent>().position.y;
	sc = this->getComponent<TransformComponent>().scale;
	//std::cout << "Updating Sprite" << std::endl;
	this->getComponent<SpriteComponent>().setSrc(50, 100);
	this->getComponent<SpriteComponent>().setDest(x, y, sc);
	//std::cout << "Finished" << std::endl;
}

void Wall::update() {
	TransformComponent *tc = &this->getComponent<TransformComponent>();
	this->getComponent<ColliderComponent>().setCollider(tc->position.x, tc->position.y);
}

//draws sprite
void Wall::draw(SDL_Rect cam) {
	//TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);
	SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
	SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
	SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
	TextureManager::Draw(tex, src, dest, cam, 0);
	//TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);
}

//sets position
void Wall::setPosition(float x, float y) {
	this->getComponent<TransformComponent>().setPos(x, y);
	this->getComponent<SpriteComponent>().setDest(x, y, this->getComponent<TransformComponent>().scale);
	TransformComponent* tc = &this->getComponent<TransformComponent>();
	this->getComponent<ColliderComponent>().setCollider(tc->position.x, tc->position.y);
}

