#include "Game.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Camera.h"
#include <mutex>
#include "SDL_mixer.h"

SDL_Rect Game::camera;

SceneManager Game::sceneManager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Window* Game::window = nullptr;
std::mutex Game::rendering;

Game::Game() {};
Game::~Game() {};

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;

	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialised!..." << std::endl;
		
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window Created" << std::endl;

			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				std::cout << "Renderer Created" << std::endl;
				isRunning = true;
			} else {
				isRunning = false;
			}
		}
	}
	camera = { 0, 0, width, height};
	//SceneManager init here
	sceneManager.init();	
}

void Game::handleEvents() {
	//Scene HandeleEvents here
	Game::sceneManager.handleEvents();

	SDL_PollEvent(&event);

	while (SDL_PollEvent(&event) != 0) {

		if (event.type == SDL_QUIT) {
			isRunning = false;
		}

	}

}

void Game::update() {
	//Scene update here
	Game::sceneManager.update();
}

void Game::render() {
	rendering.lock();
	//std::cout << "Clearing Renderer" << std::endl;
	SDL_RenderClear(renderer);
	//std::cout << "Rendering New Images" << std::endl;
	//Scene render here
	Game::sceneManager.render();
	//std::cout << "Presenting Images" << std::endl;
	SDL_RenderPresent(renderer);
	//std::cout << "Finished Rendering" << std::endl;
	rendering.unlock();
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	Mix_FreeMusic(GameScene::mixer->music);
	Mix_FreeChunk(GameScene::mixer->laserShot->sfx);
	Mix_FreeChunk(GameScene::mixer->ricochet->sfx);
	Mix_FreeChunk(GameScene::mixer->ricochet_metal->sfx);

	Mix_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

SDL_Window* Game::getWindow() {
	return window;
}
