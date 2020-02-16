#include "MenuSceneUI.h"
#include "TextureManager.h"

MenuSceneUI::MenuSceneUI() {}
MenuSceneUI::MenuSceneUI(std::string uiID, std::string path, int width, int height, float x, float y, int pixX, int pixY) {
	id = uiID;
	texture = TextureManager::LoadTexture(path.c_str());
	dest.w = width;
	dest.h = height;
	dest.x = x;
	dest.y = y;

	src.w = pixX;
	src.h = pixY;
	//Always zero to take the full image
	src.x = 0;
	src.y = 0;
}

MenuSceneUI::~MenuSceneUI() {}

SDL_Rect MenuSceneUI::getSrc() {
	return src;
}

SDL_Rect MenuSceneUI::getDest() {
	return dest;
}

SDL_Texture* MenuSceneUI::getTexture() {
	return texture;
}

std::string MenuSceneUI::getID() {
	return id;
}

bool MenuSceneUI::withinUI(int x, int y) {
	int heightMIN = dest.y; //Bottom
	int heightMAX = dest.y + (dest.h); //Top
	int widthMIN = dest.x; //Left
	int widthMAX = dest.x + (dest.w); //Right

	if (x >= widthMIN && x <= widthMAX) {
		if (y >= heightMIN && y <= heightMAX) {
			return true;
		}
	}
	return false;
}
