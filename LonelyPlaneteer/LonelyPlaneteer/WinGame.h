#pragma once

#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"

class WinGame : public Entity
{
public:
	WinGame(float x, float y);
	void update() override;
	void draw(SDL_Rect cam) override;
	void setWin() { win = true; };

private:
	bool win = false;
};