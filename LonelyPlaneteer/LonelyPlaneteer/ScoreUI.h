#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "ScoreComponent.h"
// Made by Zac Koop

class ScoreUI : public Entity {
public:
	ScoreUI();
	ScoreUI(ScoreComponent &score, TextureManager &textMng);
	void updateScore();

private:
	TextureManager* textureManager;
	ScoreComponent* scoreComponent;

};