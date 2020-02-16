#include "ShipDropzone.h"

ShipDropzone::ShipDropzone(float x, float y) {
	this->addComponent<TransformComponent>(x, y, 64, 64, 1);
	this->addComponent<ColliderComponent>("ShipWall");
}

void ShipDropzone::update() {
	TransformComponent *tc = &this->getComponent<TransformComponent>();
	this->getComponent<ColliderComponent>().setCollider(tc->position.x, tc->position.y);
}

void ShipDropzone::draw(SDL_Rect cam) {
	TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);
}