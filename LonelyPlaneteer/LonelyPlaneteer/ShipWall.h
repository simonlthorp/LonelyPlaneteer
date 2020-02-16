#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"

class ShipWall : public Entity {
private:
public:
	ShipWall(float posX, float posY, int w, int h);
	void update() override;

	void draw(SDL_Rect cam) override;
};
