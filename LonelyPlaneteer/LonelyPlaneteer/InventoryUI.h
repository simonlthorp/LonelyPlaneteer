#pragma once
#include "ECS.h"
#include "Animation.h"
#include <map>
//#include "InventoryComponent.h"
// Made by Zac Koop

class InventoryComponent;

class InventoryUI : public Entity {
public:
	InventoryUI();
	void updateInventory(int itemID);
	void draw(SDL_Rect cam) override;
	void play(const char* animName);
	void update() override;
	void setItem(int i);

private:
	int origin_x, origin_y;
	int animIndex = 0;
	int frames = 0;
	int animSpeed = 0;
	std::map<const char*, Animation> animations;
	int curItem;
};