#pragma once
#include "Component.h"
#include "Vector2D.h"
struct TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;
	int originX = 0;
	int originY = 0;
	int speed = 3;
	int height = 32;
	int width = 32;
	int scale = 1;
	float angle = 0;

	TransformComponent() {
		position.Zero();
	}

	TransformComponent(int sc) {
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int s) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = s;
	}

	~TransformComponent() {}

	void init() override {
		velocity.Zero();
	}
	//float x() { return position.x; }
	//float y() { return position.y; }

	void setPos(float x, float y) {//set the position
		position.x = x;
		position.y = y;

		originX = width / 2 + static_cast<int>(x);
		originY = height / 2 + static_cast<int>(y);
	}

};