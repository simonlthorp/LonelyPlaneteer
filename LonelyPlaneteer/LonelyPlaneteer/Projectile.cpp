#include "Projectile.h"
#include "Game.h"


Projectile::Projectile(){}

Projectile::Projectile(int mouse_x, int mouse_y) {
	
	SDL_GetWindowSize(Game::getWindow(), &origin_x, &origin_y);

	this->addComponent<TransformComponent>(origin_x/2 + Game::camera.x - 8, origin_y/2 + Game::camera.y - 8, 16, 16, 1);
	this->addComponent<SpriteComponent>("Assets/bullet_anims.png");
	this->addComponent<ColliderComponent>("Bullet");
	this->addComponent<MouseController>();

	angle_degrees = this->getComponent<MouseController>().setAngle();

	target_vector.x = mouse_x - origin_x/2.0f;
	target_vector.y = mouse_y - origin_y/2.0f;

	vector_length = sqrt(target_vector.x*target_vector.x + target_vector.y*target_vector.y);

	Animation fly = Animation(0, 1, 100);
	Animation explode = Animation(1, 3, 200);

	animations.emplace("Fly", fly);
	animations.emplace("Explode", explode);

	play("Fly");

	//Game::audioManager->laser_shoot->play();
	//GameScene::audioManager->laser_shoot->play();
	GameScene::mixer->laserShot->play();

}

Projectile::~Projectile() {

	this->destroy();

}

void Projectile::update() {

	this->getComponent<ColliderComponent>().update();

	//Animate the sprite
	this->getComponent<SpriteComponent>().setSrc(
		this->getComponent<SpriteComponent>().getSrc().w * static_cast<int>((SDL_GetTicks() / animSpeed) % frames),
		animIndex * this->getComponent<TransformComponent>().height
	);

	updatePosition();
	checkCollision();

	if (hitTime != 0){
		if ((SDL_GetTicks() - hitTime) > (animSpeed*frames)) {
			this->~Projectile();
		}
	}

}

void Projectile::updatePosition() {
	TransformComponent *tc = &this->getComponent<TransformComponent>();

	tc->position.x += (target_vector.x/vector_length) * bulletSpeed;
	tc->position.y += (target_vector.y/vector_length) * bulletSpeed;
	
	this->getComponent<SpriteComponent>().setDest(tc->position.x, tc->position.y, 1);
	this->getComponent<ColliderComponent>().setCollider(tc->position.x, tc->position.y);

}

void Projectile::draw(SDL_Rect cam) {
	SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
	SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
	SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
	if (this->isActive()) {
		TextureManager::Draw(tex, src, dest, cam, angle_degrees);
	}
	
}

void Projectile::play(const char* animName) {

	frames = animations[animName].frames;
	animIndex = animations[animName].index;
	animSpeed = animations[animName].speed;

}

void Projectile::checkCollision() {

	//Projectile hits a wall
	if (Physics::collidesWithWall(this->getComponent<ColliderComponent>().collider)) {

		this->bulletSpeed = 0.0f;
		this->updatePosition();

		//Sound FX
		if (hitTime == 0) {
			GameScene::mixer->ricochet->play();
		}
		
		killBullet();

	}

	//Projectile hits an enemy
	if (Physics::collidesWithTag(this->getComponent<ColliderComponent>().collider, "Enemy")) {

		this->bulletSpeed = 0.0f;
		this->updatePosition();

		//Sound FX
		if (hitTime == 0) {
			GameScene::mixer->ricochet_metal->play();
		}

		killBullet();

	}

	//Projectile hits the ship
	if (Physics::collidesWithShip(this->getComponent<ColliderComponent>().collider)) {

		this->bulletSpeed = 0.0f;
		this->updatePosition();

		//Sound FX
		if (hitTime == 0) {
			GameScene::mixer->ricochet_metal->play();
		}

		killBullet();

	}
}

void Projectile::killBullet() {

	play("Explode");

	if (hitTime == 0) {
		hitTime = SDL_GetTicks();
	}

}