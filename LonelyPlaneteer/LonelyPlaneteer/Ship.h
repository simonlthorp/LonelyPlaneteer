#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"
#include "ShipWall.h"
#include "ShipDropzone.h"

class Ship: public Entity {
private:
	int numberOfWalls = 7;
	ShipWall* walls[7];
	ShipDropzone* dropzone;
	Item* items[3];
	int itemIndex =  0;

public:
	Ship(float x, float y);
	void update() override;
	Entity* getWall(int x);
	void draw(SDL_Rect cam) override;
	Entity* getDropzone();
	void addItem(Item *i);
};
