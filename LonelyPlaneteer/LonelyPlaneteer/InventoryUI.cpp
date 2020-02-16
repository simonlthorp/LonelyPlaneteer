#include "InventoryUI.h"
#include "Component.h"
#include <iostream>
// Made by Zac Koop

InventoryUI::InventoryUI() {
	SDL_GetWindowSize(Game::getWindow(), &origin_x, &origin_y);
	this->addComponent<TransformComponent>(origin_x - 96, 15, 64, 64, 1);
	this->addComponent<SpriteComponent>("Assets/Inventory_SpriteSheet.png");
	int sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(origin_x - 96, 15, sc);
	this->addComponent<ColliderComponent>("InventoryUI");


	Animation empty = Animation(0, 1, 100);
	Animation gear = Animation(1, 1, 100);
	Animation crystal = Animation(2, 1, 100);
	Animation turret = Animation(3, 1, 100);

	animations.emplace("empty", empty);
	animations.emplace("gear", gear);
	animations.emplace("crystal", crystal);
	animations.emplace("turret", turret);

	play("empty");

}

void InventoryUI::draw(SDL_Rect cam) {


	SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
	SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
	SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
	TextureManager::Draw(tex, src, dest, 0);

}

void InventoryUI::setItem(int itemID) {
	curItem = itemID;
}


void InventoryUI::updateInventory(int itemID) {
	std::cout << "InventoryUI inside Update: " << curItem << std::endl;

	switch (itemID) {
	case 3:
		play("turret");
		break;
	case 2:
		play("crystal");
		break;
	case 1:
		play("gear");
		break;
	case 0:
		play("empty");
		break;
	default:
		break;
	}
}

void InventoryUI::play(const char* animName) {

	frames = animations[animName].frames;
	animIndex = animations[animName].index;
	animSpeed = animations[animName].speed;

}


void InventoryUI::update() {

	//Animate the sprite
	this->getComponent<SpriteComponent>().setSrc(
		this->getComponent<SpriteComponent>().getSrc().w * static_cast<int>((SDL_GetTicks() / animSpeed) % frames),
		animIndex * this->getComponent<TransformComponent>().height
	);

}