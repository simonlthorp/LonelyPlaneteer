#include "Physics.h"
#include "Component.h"
#include "Game.h"
#include "Ship.h"

bool Physics::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y) {
		return true;
	}
	return false;
}

bool Physics::collidesWithWall(const SDL_Rect& recA) {
	for (auto& e : GameScene::wallManager.getEntities()) {
		if (AABB(recA, e->getComponent<ColliderComponent>().collider)) {
			return true;
		}
	}
	return false;
}

std::string Physics::collidesWithEntity(const SDL_Rect& recA, std::string yourTag) {
	for (auto& e : GameScene::entityManager.getEntities() ) {
		if (AABB(recA, e->getComponent<ColliderComponent>().collider) 
				&& e->getComponent<ColliderComponent>().tag.compare(yourTag) != 0) {
			return e->getComponent<ColliderComponent>().tag;
		}
	}
	return "";
}

Entity* Physics::collidesWithTag(const SDL_Rect& recA, std::string t) {
	for (auto& e : GameScene::entityManager.getEntities()) {
		if (AABB(recA, e->getComponent<ColliderComponent>().collider) 
				&& e->getComponent<ColliderComponent>().tag.compare(t) == 0) {
			return e.get();
		}
	}
	return nullptr;
}


bool Physics::collidesWithShip(const SDL_Rect& recA) {
	for (auto& e : GameScene::shipManager.getEntities()) {
		Ship* ship = dynamic_cast<Ship*>(e.get());
		for (int i = 0; i < 7; i++) {
			if (AABB(recA, ship->getWall(i)->getComponent<ColliderComponent>().collider)) {
				return true;
			}
		}
	}
	return false;
}

bool Physics::collidesWithShipDropzone(const SDL_Rect& recA) {
	for (auto& e : GameScene::shipManager.getEntities()) {
		Ship* ship = dynamic_cast<Ship*>(e.get());
		if (AABB(recA, ship->getDropzone()->getComponent<ColliderComponent>().collider)) {
			return true;
		}
	}
	return false;
}

bool Physics::collidesWithEnemy(const SDL_Rect& recA) {
	for (auto& e : GameScene::entityManager.getEntities()) {
		if (AABB(recA, e->getComponent<ColliderComponent>().collider)
			&& e->getComponent<ColliderComponent>().tag.compare("Enemy") == 0) {
			return true;
		}
	}
	return false;
}

bool Physics::collidesWithPlayer(const SDL_Rect& recA) {
	for (auto& e : GameScene::entityManager.getEntities()) {
		if (AABB(recA, e->getComponent<ColliderComponent>().collider)
			&& e->getComponent<ColliderComponent>().tag.compare("Player") == 0) {
			return true;
		}
	}
	return false;
}


