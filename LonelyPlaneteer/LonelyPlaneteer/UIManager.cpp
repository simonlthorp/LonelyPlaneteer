#include "UIManager.h"
// Made by Zac Koop

UIManager::UIManager(){

	this->addComponent<TransformComponent>(0, 0, 0, 0, 1);
	this->addComponent<ColliderComponent>("UIManager");

	// Create UI entities for Health, Inventory, and Score
	healthUI = new HealthUI();
	GameScene::entityManager.addNewEntity(healthUI);

	//inventoryUI = new InventoryUI(*inventoryComponent, textureManager);
	inventoryUI = new InventoryUI();
	GameScene::entityManager.addNewEntity(inventoryUI);

}

void UIManager::draw(SDL_Rect cam) {

}
void UIManager::update() {

}
