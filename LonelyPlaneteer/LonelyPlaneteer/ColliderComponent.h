#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Component.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;

	TransformComponent *transform;
	ColliderComponent(std::string t) {
		tag = t;
	}

	~ColliderComponent() {}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}

	void update() override {
	}

	void setCollider(float x, float y) {
		collider.x = static_cast<int>(x);
		collider.y = static_cast<int>(y);
	}

	void setCollider(float x, float y, int w, int h) {
		collider.x = static_cast<int>(x);
		collider.y = static_cast<int>(y);
		collider.w = w;
		collider.h = h;
	}



};