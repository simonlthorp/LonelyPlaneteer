#include "Player.h"
#include "Physics.h"
#include "Animation.h"
#include "Projectile.h"
#include "HealthComponent.h"
#include "Ship.h"

#include "GameScene.h"
#include "UIManager.h"

UIManager* uiManager = nullptr;

Player::Player(float x, float y) {

	this->addComponent<TransformComponent>(x, y, 32, 32, 1);
	this->addComponent<SpriteComponent>("Assets/player_anims.png");
	this->addComponent<Input>();
	this->addComponent<MouseController>();
	this->addComponent<ColliderComponent>("Player");
	this->addComponent<InventoryComponent>();    // Player Inventory
	this->addComponent<HealthComponent>(3);      // Player Health  


	uiManager = new UIManager();


	lastShot = SDL_GetTicks();

	Animation walk = Animation(0, 8, 100);
	Animation idle = Animation(1, 4, 700);

	animations.emplace("Walk", walk);
	animations.emplace("Idle", idle);
	animate();

}

void Player::play(const char* animName) {

	frames = animations[animName].frames;
	animIndex = animations[animName].index;
	speed = animations[animName].speed;

}

void Player::eventHandle() {

	//Movement
	this->getComponent<Input>().update();
	move();
	actions();

	//Mouse control
	MouseController mc = this->getComponent<MouseController>();
	mc.update();
	if (mc.shooting) {
		shoot(mc.getMouseX(), mc.getMouseY());
	}
	
}

void Player::update() {
	this->getComponent<ColliderComponent>().update();
	
	this->getComponent<SpriteComponent>().setSrc(
		this->getComponent<SpriteComponent>().getSrc().w * static_cast<int>((SDL_GetTicks() / speed) % frames),
		animIndex * this->getComponent<TransformComponent>().height
	);

	updatePosition();
	checkCollision();
	animate();

}

//draws player's sprite
void Player::draw(SDL_Rect cam) { 
	TextureManager::DrawRect(&this->getComponent<ColliderComponent>().collider);
	SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
	SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
	SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
	TextureManager::Draw(tex, src, dest, cam, this->getComponent<MouseController>().setAngle());
}

//updates player's position based on velocity
void Player::updatePosition() {
	TransformComponent tc = this->getComponent<TransformComponent>();
	float x = tc.position.x;
	float y = tc.position.y;

	x += tc.velocity.x * tc.speed;
	y += tc.velocity.y * tc.speed;

	if (tc.velocity.x == 0 && tc.velocity.y == 0) {
		setStateTo(0);
	} else {
		setStateTo(1);
	}

	this->getComponent<TransformComponent>().setPos(x, y);
	this->getComponent<SpriteComponent>().setDest(x, y, tc.scale);
	this->getComponent<ColliderComponent>().setCollider(x, y);
	this->getComponent<InventoryComponent>().setPickupZone(x, y);
	
}

void Player::checkCollision() {

	if (Physics::collidesWithWall(this->getComponent<ColliderComponent>().collider)) {
		this->getComponent<TransformComponent>().velocity * -1;
		this->setStateTo(0);
		this->updatePosition();
		//std::cout << "Wall Collision!" << std::endl;
	}

	if (Physics::collidesWithEnemy(this->getComponent<ColliderComponent>().collider)) {
		
		this->getComponent<TransformComponent>().velocity * -1;
		this->updatePosition();
		damagePlayer();

	}

	if (Physics::collidesWithShip(this->getComponent<ColliderComponent>().collider)) {
		this->getComponent<TransformComponent>().velocity * -1;
		this->setStateTo(0);
		this->updatePosition();
		//std::cout << "Ship Collision!" << std::endl;
	}
	
}

//sets player position
void Player::setPosition(const float x, const float y) {
	this->getComponent<TransformComponent>().setPos(x, y);
	this->getComponent<SpriteComponent>().setDest(x, y, this->getComponent<TransformComponent>().scale);
}

void Player::setStateTo(const int s) {
	state = s;
}

void Player::animate() {
	switch (state) {
		case 0:
			play("Idle");
			break;
		case 1:
			play("Walk");
			break;
		default:
			play("Idle");
			break;
	}
}

//Checks if any keys are down
void Player::move() {
	Input input = this->getComponent<Input>();

	if (!input.keyW && !input.keyS) {
		getComponent<TransformComponent>().velocity.y = 0;
	}

	if (!input.keyA && !input.keyD) {
		getComponent<TransformComponent>().velocity.x = 0;
	}

	if (input.keyW) {
		getComponent<TransformComponent>().velocity.y = -1;
	}

	if (input.keyA) {
		getComponent<TransformComponent>().velocity.x = -1;
	}

	if (input.keyS) {
		getComponent<TransformComponent>().velocity.y = 1;
	}

	if (input.keyD) {
		getComponent<TransformComponent>().velocity.x = 1;
	}
}

void Player::actions() {
	if ((SDL_GetTicks() - lastETime) > 200) {
		Input input = this->getComponent<Input>();

		if (input.keyE) {
			if (this->getComponent<InventoryComponent>().itemSlotEmpty()) {
				std::cout << "Pick UP" << std::endl;
				tryToPickUp();
			} else {
				std::cout << "Drop" << std::endl;

				// update UI to empty inventory
				if (!this->getComponent<InventoryComponent>().itemSlotEmpty()) {
					uiManager->inventoryUI->updateInventory(0);  // update inventoryUI
				}
				//try to drop Item
				tryToDrop();
			}
		}
		lastETime = SDL_GetTicks();
	}
}

void Player::setManager(Manager *manager) {
	this->manager = manager;
	std::cout << "player manager set to: " << manager << std::endl;
}

//Pick up item
void Player::tryToPickUp() {
	InventoryComponent *inventory = &this->getComponent<InventoryComponent>();
	TransformComponent *tc = &this->getComponent<TransformComponent>();
	Item* item = dynamic_cast<Item*>(Physics::collidesWithTag(inventory->pickupZone, "Item"));
	if (item != nullptr) {
		inventory->pickUpItem(item);
		item->getPickUpped();
		std::cout << tc->position.x << " " << tc->position.y << std::endl;


		// update UI to contain new item in slot.
		std::cout << "ITEM ID: " << this->getComponent<InventoryComponent>().getItemID() << std::endl;
		uiManager->inventoryUI->updateInventory(this->getComponent<InventoryComponent>().getItemID());  // update inventoryUI
	}
}

void Player::tryToDrop() {
	if (Physics::collidesWithShipDropzone(this->getComponent<ColliderComponent>().collider)) {
		InventoryComponent *inventory = &this->getComponent<InventoryComponent>();
		Item* item = inventory->getItem();

		dropItemInShip(item);

		inventory->dropItem();
	}
}


void Player::dropItemInShip(Item *i) {
	for (auto& e : GameScene::shipManager.getEntities()) {
		Ship* ship = dynamic_cast<Ship*>(e.get());

		ship->addItem(i);

		TransformComponent *tc = &this->getComponent<TransformComponent>();
		std::cout << tc->position.x << " " << tc->position.y << std::endl;
		i->setPosition(tc->position.x, tc->position.y);
		i->getDropped();
		
	}
}

//Shoot the weapon in the direction of the mouse cursor
void Player::shoot(int mouse_x, int mouse_y) {



	if ((SDL_GetTicks() - lastShot) > 200) {

		Projectile* bullet = nullptr;

		bullet = new Projectile(mouse_x, mouse_y);

		GameScene::entityManager.addNewEntity(bullet);
		bullet->update();
		lastShot = SDL_GetTicks();
		
	}

}

void Player::damagePlayer() {
	//std::cout << "Damage Player" << std::endl;
	//std::cout << lastDamage - 500 << std::endl;
	if (SDL_GetTicks() > (lastDamage + 500)) {

		this->getComponent<HealthComponent>().damageHealth(1);
		std::cout << "Health: " << this->getComponent<HealthComponent>().getHealth() << std::endl;
		lastDamage = SDL_GetTicks();
		uiManager->healthUI->updateHealth(this->getComponent<HealthComponent>().getHealth());  // update healthUI
	}

	if (this->getComponent<HealthComponent>().getHealth() <= 0) {

		//killPlayer();

	}

}
