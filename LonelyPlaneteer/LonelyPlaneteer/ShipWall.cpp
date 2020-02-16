#include "ShipWall.h"

ShipWall::ShipWall(float posX, float posY, int h, int w) {
	this->addComponent<TransformComponent>(posX, posY, w, h, 1);
	this->addComponent<ColliderComponent>("ShipWall");
}

void ShipWall::update() {
	TransformComponent *tc = &this->getComponent<TransformComponent>();
	this->getComponent<ColliderComponent>().setCollider(tc->position.x, tc->position.y);
}

void ShipWall::draw(SDL_Rect cam) {
	TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);
}