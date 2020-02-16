#pragma once
#include "SDL.h"
#include "Wall.h"
#include "ECS.h"
class Physics {

public:
	//static Wall wall;
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool collidesWithWall(const SDL_Rect& recA);
	static std::string collidesWithEntity(const SDL_Rect& recA, std::string yourTag);
	static Entity* collidesWithTag(const SDL_Rect& recA, std::string t);
	static bool collidesWithShip(const SDL_Rect& recA);
	static bool collidesWithEnemy(const SDL_Rect& recA);
	static bool collidesWithPlayer(const SDL_Rect& recA);
	static bool collidesWithShipDropzone(const SDL_Rect& recA);
};