#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
//#include "Component.h"
//#include "HealthComponent.h"
// Made by Zac Koop

class HealthComponent;

class HealthUI : public Entity {
public:
	HealthUI();
	void updateHealth(int h);
	void draw(SDL_Rect cam) override;
	void play(const char* animName);
	void update() override;
	void setHealth(int h);

private:
	int animIndex = 0;
	int frames = 0;
	int animSpeed = 0;
	std::map<const char*, Animation> animations;
	int currentHealth;

};