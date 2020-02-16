#include "TextureManager.h"
#include "Camera.h"

SDL_Texture* TextureManager::LoadTexture(const char* filename) {
	Game::rendering.lock();
	//std::cout << "Loading Image" << std::endl;
	SDL_Surface* tempSurface = IMG_Load(filename);
	//std::cout << "Creating Texture From Surface" << std::endl;
	SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	//std::cout << "Freeing Surface" << std::endl;
	SDL_FreeSurface(tempSurface);
	//std::cout << "Returning Texture" << std::endl;
	Game::rendering.unlock();
	return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, double degrees)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, degrees, NULL, SDL_FLIP_NONE);
}

//Possible Critical Section
//std::mutex loadingPath;
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_Rect cam, double degrees) {
	//loadingPath.lock();
	SDL_Rect destination = { dest.x - cam.x, dest.y - cam.y, dest.w, dest.h };
	//SDL_RenderCopyEX(Game::renderer, tex, NULL, &dest);
	SDL_RenderCopyEx(Game::renderer, tex, &src, &destination, degrees, NULL, SDL_FLIP_NONE);
	//loadingPath.unlock();
}



void TextureManager::DrawRect(SDL_Rect* rect) {
	//SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 0);
	//SDL_RenderFillRect(Game::renderer, rect);
	//SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
}