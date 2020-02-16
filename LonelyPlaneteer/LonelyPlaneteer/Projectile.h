#pragma once

#include "ECS.h"
#include "Component.h"
#include <map>
#include "Animation.h"
#include "TextureManager.h"
#include "Physics.h"
#include "Enemy.h"

class Projectile : public Entity
{
public:
	Projectile();
	Projectile(int mouse_x, int mouse_y);
	~Projectile();

	void update() override;

	void updatePosition();

	void draw(SDL_Rect cam);

	void play(const char* animName);

	void checkCollision();

	void killBullet();

private:
	int animIndex = 0;
	std::map<const char*, Animation> animations;
	int frames = 0;
	int animSpeed = 0;
	float bulletSpeed = 6.0f;
	int origin_x;
	int origin_y;
	double angle_degrees;
	Vector2D target_vector;
	double vector_length;
	Uint32 hitTime = 0;
};
