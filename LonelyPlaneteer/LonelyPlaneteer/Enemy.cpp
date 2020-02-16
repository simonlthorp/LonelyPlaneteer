#include "Enemy.h"
#include <math.h>
#include <time.h>
#include "Animation.h"

Enemy::Enemy(float x, float y, Player * p, MapManager * mm) {
	this->addComponent<TransformComponent>(x, y, 42, 46, 1);
	this->addComponent<SpriteComponent>("Assets/enemy_anims.png");
	this->addComponent<ColliderComponent>("Enemy");
	this->addComponent<SightComponent>();
	this->p = p;
	this->mm = mm;
	
	this->getComponent<TransformComponent>().speed = 2;
	state = EnemyState::WANDER;
	goalX = x;
	goalY = y;

	Animation walk = Animation(0, 8, 100);
	Animation attack = Animation(1, 5, 100);
	Animation explode = Animation(2, 6, 100);

	animations.emplace("Walk", walk);
	animations.emplace("Attack", attack);
	animations.emplace("Explode", explode);

	play("Walk");

	path = { -1, -1 };
}

Enemy::~Enemy() {

	//std::cout << "Enemy Destroyed" << std::endl;

}

void Enemy::update() {
	TransformComponent tc = this->getComponent<TransformComponent>();
	path = { -1, -1 };

	//Face towards your goal
	face();

	//Calculates the forward vector for the enemy
	this->getComponent<SightComponent>().calculateSightLine(&this->getComponent<TransformComponent>(), GIVE_UP_DISTANCE);

	if (this->getComponent<SightComponent>().canSeePlayer(&this->getComponent<TransformComponent>(), &p->getComponent<TransformComponent>(), GIVE_UP_DISTANCE)) {
		state = EnemyState::CHASE;
	} else {
		state = EnemyState::WANDER;
	}

	switch (state) {
	case EnemyState::CHASE:
		chase(p);
		break;
	case EnemyState::ATTACK:
		//attack()
		break;
	case EnemyState::PATROL:
		patrol();
		break;
	case EnemyState::WANDER:
		//only change the new goal when you reach the new one
		if (abs(goalX - this->getComponent<TransformComponent>().position.x) <= 10 && abs(goalY - this->getComponent<TransformComponent>().position.y) <= 10) {
			wander();
		}
		break;
	default:
		break;
	}

	//Animate the sprite
	this->getComponent<SpriteComponent>().setSrc(
		this->getComponent<SpriteComponent>().getSrc().w * static_cast<int>((SDL_GetTicks() / animSpeed) % frames),
		animIndex * this->getComponent<TransformComponent>().height
	);

	checkCollision();

	if (!collidingWithPlayer && !isDead) {
		move();
		play("Walk");
	}
	
	//Move the enemy off screen when they are dead.
	if (isDead) {
		//move the collider immediately so we can walk through without causing a collision
		this->getComponent<ColliderComponent>().setCollider(DEATH_LOCATION, DEATH_LOCATION);

		//wait for the death animation to finish
		if (diedAtTime < SDL_GetTicks() - (animSpeed * frames)) {
			//move the rest of the stuff to the new death location.
			setPosition(DEATH_LOCATION, DEATH_LOCATION);
		}
	}
}

void Enemy::draw(SDL_Rect cam) {

	if (this->isDead) {
		return;
	}

	TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);
	SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();//get destination rect
	SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();//get source rect 
	SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture(); // get the texture
	TextureManager::Draw(tex, src, dest, cam, this->getComponent<TransformComponent>().angle * 180.00f / M_PI - 90.00f);

	float tmpX = this->getComponent<TransformComponent>().position.x + this->getComponent<SpriteComponent>().getDest().w / 2;
	float tmpY = this->getComponent<TransformComponent>().position.y + this->getComponent<SpriteComponent>().getDest().h / 2;
}

void Enemy::updatePosition() {}

void Enemy::setPosition(float x, float y) {
	this->getComponent<ColliderComponent>().setCollider(x, y);
	this->getComponent<TransformComponent>().setPos(x, y);
	this->getComponent<SpriteComponent>().setDest(x, y, this->getComponent<TransformComponent>().scale);
}

void Enemy::setPath(int x, int y) {
	this->path.x = x;
	this->path.y = y;
}

bool Enemy::getIsDead()
{
	return this->isDead;
}

void Enemy::move() {
	TransformComponent *  tc = &this->getComponent<TransformComponent>();
	SpriteComponent * sc = &this->getComponent<SpriteComponent>();

	float x = tc->position.x;
	float y = tc->position.y;

	if (x > goalX) {
		tc->velocity.x = -1;
	}

	if (x < goalX) {
		tc->velocity.x = 1;
	}

	if (y > goalY) {
		tc->velocity.y = -1;
	}

	if (y < goalY) {
		tc->velocity.y = 1;
	}

	if (x == goalX) {
		tc->velocity.x = 0;
	}

	if (y == goalY) {
		tc->velocity.y = 0;
	}

	x += tc->velocity.x * tc->speed;
	y += tc->velocity.y * tc->speed;

	setPosition(x, y);
}

void Enemy::chase(const Player * player) {
	if (player == nullptr) {
		std::cerr << "Player is null!" << std::endl;
		return;
	} else {
		if (path.x != -1) {
			goalX = path.x;
			goalY = path.y;
		} else {
			const TransformComponent pT = player->getComponent<TransformComponent>();
			goalX = pT.position.x;
			goalY = pT.position.y;
		}
	}
}

void Enemy::wander() {
	TransformComponent * tc = &this->getComponent<TransformComponent>();
	float circleX, circleY;
	float angle;

	const int scale = mm->getMapScale();
	const int mapWidth = mm->getMapWidth() * scale * 32;
	const int mapHeight = mm->getMapHeight() * scale * 32;

	angle = static_cast<float>(rand()) / static_cast<float>(M_PI * 3 / 2);

	circleX = cos(angle) * WANDER_RADIUS;
	circleY = sin(angle) * WANDER_RADIUS;

	//Don't let the AI path outside of the game area.
	if (tc->position.x + circleX < 0 || tc->position.x + circleX >= mapWidth) {
		circleX *= -1;
	}

	if (tc->position.y + circleY < 0 || tc->position.y + circleY >= mapHeight) {
		circleY *= -1;
	}

	goalX = tc->position.x + circleX;
	goalY = tc->position.y + circleY;
}

void Enemy::patrol() {}

void Enemy::face() {
	TransformComponent * tc = &this->getComponent<TransformComponent>();
	float deltaX = goalX - tc->position.x;
	float deltaY = goalY - tc->position.y;

	//The angle in radians
	tc->angle = atan2(deltaY, deltaX);
}

void Enemy::play(const char* animName) {

	frames = animations[animName].frames;
	animIndex = animations[animName].index;
	animSpeed = animations[animName].speed;

}

void Enemy::checkCollision() {

	TransformComponent *  tc = &this->getComponent<TransformComponent>();

	//Enemy hits a wall
	if (Physics::collidesWithWall(this->getComponent<ColliderComponent>().collider)) {

	}

	//Enemy Hit by bullet
	if (Physics::collidesWithTag(this->getComponent<ColliderComponent>().collider, "Bullet")) {
		killEnemy();
	}

	//Enemy hit player
	if (Physics::collidesWithPlayer(this->getComponent<ColliderComponent>().collider)) {

		TransformComponent *  tc = &this->getComponent<TransformComponent>();
		tc->velocity.x = 0;
		tc->velocity.y = 0;
		hitPlayer();

	}
	else {
		collidingWithPlayer = false;
	}

	//Enemy hits the ship
	if (Physics::collidesWithShip(this->getComponent<ColliderComponent>().collider)) {

	}
}

void Enemy::hitPlayer() {
	playerCollideTime = SDL_GetTicks();
	play("Attack");
	collidingWithPlayer = true;
}

void Enemy::killEnemy() {
	play("Explode");
	diedAtTime = SDL_GetTicks();
	isDead = true;
}