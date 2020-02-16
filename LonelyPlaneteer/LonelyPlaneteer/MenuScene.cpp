#include "MenuScene.h"
#include "TextureManager.h"


MenuScene::MenuScene() {}
MenuScene::~MenuScene() {}


void MenuScene::init() {
	MenuSceneUI tmp = MenuSceneUI("button","Assets/Loading/start.png", 200, 50, ((Game::camera.w / 2) - (200 / 2)), ((Game::camera.h / 2) - (50 / 2)), 691, 150);
	uiComponents.push_back(tmp);

	title = TextureManager::LoadTexture("Assets/Loading/Title.png");
	destTitle.w = 800;
	destTitle.h = 124;
	//destTitle.w = Game::camera.w;
	//destTitle.h = Game::camera.h;
	destTitle.x = (Game::camera.w / 2) - (destTitle.w / 2);
	destTitle.y = (Game::camera.h / 2) - (destTitle.h / 2) - 100;

	srcTitle.w = 800;
	srcTitle.h = 124;
	srcTitle.x = 0;
	srcTitle.y = 0;
}

void MenuScene::handleEvents() {
	updateMouse();
}
void MenuScene::update() {}

void MenuScene::render() {
	for (int i = 0; i < uiComponents.size(); i++) {
		MenuSceneUI tmp = uiComponents.at(i);
		TextureManager::Draw(tmp.getTexture(), tmp.getSrc(), tmp.getDest(), Game::camera, 0);
	}
	TextureManager::Draw(title, srcTitle, destTitle, Game::camera, 0);
	//printf("Render Loading\n");
}

//Mouse
void MenuScene::updateMouse() {
	//Shoot at mouse cursor
	if (SDL_GetMouseState(&mouse_x, &mouse_y) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
		//left click
		//std::cout << getMouseX() << " " << getMouseY() << std::endl;
		for (int i = 0; i < uiComponents.size(); i++) {
			if (uiComponents.at(i).withinUI(getMouseX(), getMouseY())) {
				if (uiComponents.at(i).getID() == "button") {
					//Calls the Scene Manager to switch to next Scene
					SceneManager::incrementLoadGame();
					//SceneManager::loadGame = 1;
				}
				std::cout << uiComponents.at(i).getID() << " Started Loading" << std::endl;
			}
		}
	}
	else {
		//nothing??
	}
}

int MenuScene::getMouseX() {
	return mouse_x;
}

int MenuScene::getMouseY() {
	return mouse_y;
}