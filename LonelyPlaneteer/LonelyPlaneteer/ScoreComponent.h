#pragma once
#include "ECS.h"
// Made by Zac Koop

class ScoreComponent : public Component {
private:
	int score;

public:
	ScoreComponent();
	ScoreComponent(int s) { score = s; };

	int getScore() { return score; };

	void addScore(int s) { score += s; };
};


