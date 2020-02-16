#pragma once

#include "Component.h"
#include "Game.h"
#include "Projectile.h"


class MouseController : public Component
{
public:
	bool shooting = false;

	void init() {

		SDL_GetWindowSize(Game::getWindow(), &origin_x, &origin_y);

		origin_x = origin_x / 2;
		origin_y = origin_y / 2;

	}

	void update() override {

		//Shoot at mouse cursor
		if (SDL_GetMouseState(&mouse_x, &mouse_y) && SDL_BUTTON(SDL_BUTTON_LEFT)) {

			shooting = true;

		}
		else {
			shooting = false;
		}
	}

	//Calculates difference between mouse location and player location
	void setDelta() {

		SDL_GetMouseState(&mouse_x, &mouse_y);

		delta_x = origin_x - mouse_x;
		delta_y = origin_y - mouse_y;

	}

	//Calculates the angle to point player at mouse cursor
	double setAngle() {

		setDelta();

		//angle_degrees = fmod(atan2(delta_y, delta_x) + 2 * M_PI, 2 * M_PI);

		//angle_degrees * 180.00f / M_PI;
		//return angle_degrees;

		angle_degrees = (atan2(delta_y, delta_x)*180.0000) / M_PI;

		angle_degrees += 90.0f;

		return angle_degrees;

	}

	int getMouseX() {
		return mouse_x;
	}

	int getMouseY() {
		return mouse_y;
	}


private:
	double angle_degrees;
	double delta_x;
	double delta_y;
	int mouse_x;
	int mouse_y;
	int origin_x;//player start position x coord
	int origin_y;//player start position y coord
	

};
