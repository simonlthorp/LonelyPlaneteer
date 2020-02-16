#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "MapManager.h"

/*------------------------------------------------------------------------------------------------
-- SOURCE FILE: Enemy.h
--
-- FUNCTIONS:   Enemy(float, float, Player *)
--              void update() override
--              void draw(SDL_Rect cam) override
--              void updatePosition()
--              void setPosition(float, float)
--              void move()
--              void chase(const Player *)
--              void patrol()
--              void wander()
--              void face()
--
--
-- DATE:        October 15, 2017
--
-- DESIGNER:    Maitiu Morton, Simon Thorp & Michael Goll
--
-- PROGRAMMER:  Maitiu Morton & Michael Goll
--
-- NOTES:       Class for the "Enemy" character type in the game.
--              Handles all operations of the enemies in the game.
--------------------------------------------------------------------------------------------------*/

#include "Component.h"
#include "Player.h"
#include "PathNode.h"
#include "EnemyState.h"

#define CHASE_DISTANCE 200
#define GIVE_UP_DISTANCE 300
#define WANDER_RADIUS 300
#define DEATH_LOCATION -300

class Player;

class Enemy : public Entity {
private:
	Player * p;
	EnemyState state;
	MapManager * mm;

	float goalX, goalY;
	Uint32 playerCollideTime;
	bool collidingWithPlayer = false;
	Uint32 diedAtTime = 0;
	bool isDead = false;

	//Animation Variables
	int animIndex = 0;
	int frames = 0;
	int animSpeed = 0;
	std::map<const char*, Animation> animations;

	//Pathfinding variables
	//std::vector<PathNode*> path;
	//SDL_Point followPoint;
	SDL_Point path;

public:
	Enemy(float x, float y, Player *, MapManager *);

	~Enemy();

	void update() override;

	void draw(SDL_Rect cam) override;

	void updatePosition();

	void setPosition(float x, float y);

	void setPath(int x, int y);

	bool getIsDead();

	void move();

	void chase(const Player *);

	void patrol();

	void wander();

	void face();

	void checkCollision();

	void hitPlayer();

	void killEnemy();

	//Animation
	void play(const char* animName);

};