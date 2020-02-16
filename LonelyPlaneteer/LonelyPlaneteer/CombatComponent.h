#pragma once
#include "ECS.h"

class Health : public Component {
private:
	int health;

public:
	Health(int h) { health = h };

	int getHealth() { return health; };

	void damageHealth(int d) { health -= d; };
}; 
