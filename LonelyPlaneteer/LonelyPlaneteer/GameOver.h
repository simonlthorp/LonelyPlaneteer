#pragma once

#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"

class GameOver : public Entity
{
public:
	GameOver(float x, float y);
	void update() override;
	void draw(SDL_Rect cam) override;
	void setGameOver() { over = true; };

private:
	bool over = false;
};