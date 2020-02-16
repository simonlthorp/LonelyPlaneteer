#include "Item.h"
#include "Component.h"
Item::Item(float x, float y, int t, std::string spritePath) {
	int sc;
	const char * path = spritePath.c_str();
	this->addComponent<TransformComponent>(x, y);
	this->addComponent<ColliderComponent>("Item");
	this->addComponent<SpriteComponent>(path);
	type = t;

	//set destination Rect for Sprite
	sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(x, y, sc);
}

void Item::update() {
	float destX;
	float destY;
	int sc;

	TransformComponent *tc = &this->getComponent<TransformComponent>();

	this->getComponent<ColliderComponent>().setCollider(tc->position.x, tc->position.y);

	destX = this->getComponent<TransformComponent>().position.x;
	destY = this->getComponent<TransformComponent>().position.y;
	sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(destX, destY, sc);
}

void Item::draw(SDL_Rect cam) {
	if (!pickedUp) {
		//TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);

		SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
		SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
		SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
		TextureManager::Draw(tex, src, dest, cam, 0);
	}
}

void Item::setPosition(const float x, const float y) {
	TransformComponent * tc = &this->getComponent<TransformComponent>();
	tc->position.x = x;
	tc->position.y = y;
}