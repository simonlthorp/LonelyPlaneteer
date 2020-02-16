#include "ScoreUI.h"
// Made by Zac Koop


ScoreUI::ScoreUI() : scoreComponent(0), textureManager(0) {}

ScoreUI::ScoreUI(ScoreComponent &score, TextureManager &textMng) : scoreComponent(&score), textureManager(&textMng) {

}

void ScoreUI::updateScore() {

}
