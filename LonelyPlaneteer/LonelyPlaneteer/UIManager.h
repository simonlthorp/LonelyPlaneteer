#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "InventoryUI.h"
#include "ScoreUI.h"
#include "HealthUI.h"
#include "Component.h"

// Made by Zac Koop
class HealthComponent;
class InventoryComponent;
class ScoreComponent;
class InventoryUI;
class HealthUI;
class ScoreUI;

class UIManager : public Entity {
public:
	UIManager();
	void draw(SDL_Rect cam) override;
	void update() override;

	HealthUI* healthUI;
	InventoryUI* inventoryUI;


private:

	TextureManager textureManager;
	HealthComponent* healthComponent;
	InventoryComponent* inventoryComponent;
	ScoreComponent* scoreComponent;

};