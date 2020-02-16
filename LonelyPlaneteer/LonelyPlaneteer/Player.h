#pragma once
#include "ECS.h"
#include "TextureManager.h"
#include "Component.h"
#include "Item.h"
#include <map>


class Player : public Entity {
private:
	int animIndex = 0;
	std::map<const char*, Animation> animations;
	int frames = 0;
	int speed = 100;
	Manager* manager;
	Uint32 lastShot;
	Uint32 lastDamage = 0;
	bool isDead = false;
	unsigned int lastETime = 0;

	int state = 0;
public:
	Player(float x, float y);
	void eventHandle();
	void update() override;

	void draw(SDL_Rect cam) override;

	void updatePosition();

	void setPosition(const float x, const float y);

	void move();

	void animate();

	void play(const char* animName);

	void shoot(int mouse_x, int mouse_y);

	void setManager(Manager *manager);

	void setStateTo(const int s);

	void checkCollision();

	void actions();

	void tryToPickUp();

	void damagePlayer();

	//void killPlayer();

	void tryToDrop();

	void dropItemInShip(Item* i);
};