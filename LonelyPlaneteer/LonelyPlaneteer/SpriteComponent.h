#pragma once

#include "Component.h"
#include "SDL.h"
#include "TextureManager.h"

class SpriteComponent : public Component {
public:
	SpriteComponent() = default;
	//Avoid doing this as its costly
	SpriteComponent(const char* path) {
		//std::cout << "Loading Texture" << std::endl;
		texture = TextureManager::LoadTexture(path);
		//std::cout << "Sprite Finished" << std::endl;
	}

	SpriteComponent(SDL_Texture* text) {
		//std::cout << "Loading Texture" << std::endl;
		texture = text;
		//std::cout << "Sprite Finished" << std::endl;
	}

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	void init() override {
		src.x = src.y = 0;
		src.w = entity->getComponent<TransformComponent>().width;
		src.h = entity->getComponent<TransformComponent>().height;
	}

	void setDest(const float x, const float y, const int scale) {
		dest.x = static_cast<int>(x);
		dest.y = static_cast<int>(y);
		dest.w = src.w * scale;
		dest.h = src.h * scale;
	}
	
	SDL_Rect getDest() const {
		return dest;
	}

	SDL_Rect getSrc() const {
		return src;
	}

	SDL_Texture* getTexture() const {
		return texture;
	}

	void setTexture(SDL_Texture* tex) {
		texture = tex;
	}

	void setSrc(int x, int y) {
		src.x = x;
		src.y = y;
	}

private:
	TransformComponent *position;
	SDL_Texture *texture;
	SDL_Rect src, dest;
};