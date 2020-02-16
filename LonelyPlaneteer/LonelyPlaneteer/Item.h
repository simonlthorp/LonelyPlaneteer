#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include <string>

class Item : public Entity {
private:
	int type;
	bool pickedUp = false;
public:
	Item(float x, float y, int t, std::string spritePath);
	void update() override;

	void draw(SDL_Rect cam) override;

	void setPosition(const float x, const float y);

	int getType() { return type; };
	void getPickUpped() { pickedUp = true; };
	void getDropped() { pickedUp = false; };
};