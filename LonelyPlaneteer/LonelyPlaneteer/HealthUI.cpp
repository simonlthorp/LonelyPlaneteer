#include "HealthUI.h"
#include "Component.h"
#include <iostream>
// Made by Zac Koop

HealthUI::HealthUI() {
	this->addComponent<TransformComponent>(15, 15, 32, 96, 2);
	this->addComponent<SpriteComponent>("Assets/Health_SpriteSheet.png");
	int sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(15, 15, sc);
	this->addComponent<ColliderComponent>("Health");

	//currentHealth = 3;

	Animation health3 = Animation(0, 1, 100);
	Animation health2 = Animation(1, 1, 100);
	Animation health1 = Animation(2, 1, 100);
	Animation health0 = Animation(3, 1, 100);

	animations.emplace("health3", health3);
	animations.emplace("health2", health2);
	animations.emplace("health1", health1);
	animations.emplace("health0", health0);

	play("health3");

}

void HealthUI::draw(SDL_Rect cam) {


		SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
		SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
		SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
		TextureManager::Draw(tex, src, dest, 0);

} 

void HealthUI::setHealth(int h) {
	currentHealth = h;
}


void HealthUI::updateHealth(int health) {
	std::cout << "HEALTH INSIDE UPDATE HEALTH: " << currentHealth << std::endl;

	switch (health) {
	case 3:
		play("health3");
		break;
	case 2:
		play("health2");
		break;
	case 1:
		play("health1");
		break;
	default:
		play("health0");
		break;
	}
}

void HealthUI::play(const char* animName) {

	frames = animations[animName].frames;
	animIndex = animations[animName].index;
	animSpeed = animations[animName].speed;

}

void HealthUI::update() {

	//Animate the sprite
	this->getComponent<SpriteComponent>().setSrc(
		this->getComponent<SpriteComponent>().getSrc().w * static_cast<int>((SDL_GetTicks() / animSpeed) % frames),
		animIndex * this->getComponent<TransformComponent>().height
	);

}