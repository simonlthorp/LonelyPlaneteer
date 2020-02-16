#include "Ship.h"

Ship::Ship(float x, float y) {
	float destX;
	float destY;
	int sc;
	this->addComponent<TransformComponent>(x, y, 384, 608, 1);
	this->addComponent<SpriteComponent>("Assets/TempShip.png");
	this->addComponent<ColliderComponent>("Ship");
	items[0] = nullptr;
	items[1] = nullptr;
	items[2] = nullptr;
	walls[0] = new ShipWall(x + 10, y + 120, 240, 150); //Rear Hull
	walls[1] = new ShipWall(x + 50, y + 20, 200, 90); //Top Engine
	walls[2] = new ShipWall(x + 50, y + 280, 200, 90); //Bottom Engine
	walls[3] = new ShipWall(x + 320, y + 90, 70, 32); //Top by door
	walls[4] = new ShipWall(x + 320, y + 260, 70, 32); //bottom by door
	walls[5] = new ShipWall(x + 380, y + 135, 190, 120); //nose body
	walls[6] = new ShipWall(x + 570, y + 160, 32, 70); //nose tip

	dropzone = new ShipDropzone(x + 250, y + 160);

	destX = this->getComponent<TransformComponent>().position.x;
	destY = this->getComponent<TransformComponent>().position.y;
	sc = this->getComponent<TransformComponent>().scale;
	this->getComponent<SpriteComponent>().setDest(x, y, sc);
}

void Ship::update() {
	TransformComponent *tc = &this->getComponent<TransformComponent>();
	dropzone->update();
	for (int i = 0; i < numberOfWalls; i++) {
		walls[i]->update();
	}
}

void Ship::draw(SDL_Rect cam) {
	SDL_Rect dest = this->getComponent<SpriteComponent>().getDest();
	SDL_Rect src = this->getComponent<SpriteComponent>().getSrc();
	SDL_Texture* tex = this->getComponent<SpriteComponent>().getTexture();
	TextureManager::Draw(tex, src, dest, cam, 0);
	dropzone->draw(cam);
	for (int i = 0; i < numberOfWalls; i++) {
		//walls[i]->draw(cam);
	}
}

Entity* Ship::getDropzone() {
	return dropzone;
}

Entity* Ship::getWall(int x) {
	return walls[x];
}

void Ship::addItem(Item *i) {
	if (itemIndex < 3) {
		std::cout << i->getType() << " Item added to Ship" << std::endl;
		items[itemIndex];
		itemIndex++;
		if (itemIndex >= 3) {

			GameScene::shipBuilt = true;
		}
	}
}