#pragma once
#include "ECS.h"

class HealthComponent : public Component {
private:
	int health;

public:
	HealthComponent();
	HealthComponent(int h) { health = h; };

	int getHealth() { return health; };

	void damageHealth(int d) { health -= d; };
};