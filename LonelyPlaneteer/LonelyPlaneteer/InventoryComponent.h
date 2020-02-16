#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Component.h"
#include "Item.h"

class InventoryComponent : public Component {
private:
	Item *itemSlot = nullptr;

public:
	SDL_Rect pickupZone;
	int size = 20;
	void init() override {
		pickupZone.x =static_cast<int>(entity->getComponent<TransformComponent>().position.x) - Game::camera.x - size / 2;
		pickupZone.y = static_cast<int>(entity->getComponent<TransformComponent>().position.y) - Game::camera.y - size / 2;
		pickupZone.w = entity->getComponent<TransformComponent>().width + size;
		pickupZone.h = entity->getComponent<TransformComponent>().height + size;
	}

	void update() override {
	}

	void setPickupZone(float x, float y) {
		//pickupZone.x = static_cast<int>(x) - Game::camera.x - size / 2;
		//pickupZone.y = static_cast<int>(y) - Game::camera.y - size / 2;
		pickupZone.x = x - size / 2;
		pickupZone.y = y - size / 2;
	}

	bool itemSlotEmpty() { return itemSlot == nullptr; }

	void pickUpItem(Item *item) { itemSlot = item; }

	void dropItem() { itemSlot = nullptr; }

	Item* getItem() { return itemSlot;	}

	int getItemID() { return itemSlot->getType(); }
};
