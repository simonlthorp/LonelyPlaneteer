#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"

class Wall : public Entity {
private:

public:
	Wall(SDL_Texture* texture, float x, float y);
	void update() override;

	void draw(SDL_Rect cam) override;

	void setPosition(float x, float y);
};
