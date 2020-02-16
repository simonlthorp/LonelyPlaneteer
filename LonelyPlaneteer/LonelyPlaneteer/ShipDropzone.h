#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"

class ShipDropzone : public Entity {
private:
public:
	ShipDropzone(float posX, float posY);
	void update() override;

	void draw(SDL_Rect cam) override;
};
