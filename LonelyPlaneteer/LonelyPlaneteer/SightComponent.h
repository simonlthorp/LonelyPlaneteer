#pragma once
/*------------------------------------------------------------------------------------------------
-- SOURCE FILE: SightComponent.h
--
-- FUNCTIONS:  bool canSeePlayer(const TransformComponent *, const TransformComponent *, const float)
--
-- DATE:        October 18, 2017
--
-- DESIGNER:    Michael Goll
--
-- PROGRAMMER:  Michael Goll
--
-- NOTES:       Gives an entity "vision" and can be used to determine whether a character/object
--                  can see another character/object.
--              The target character/object MUST have a TransformComponent component in order to be seen.
--------------------------------------------------------------------------------------------------*/

#include "Component.h"
#include "Vector2D.h"
#include "Game.h"

#define PERIF_VISION 180

struct SightComponent : public Component {
private:

	float fieldOfView;

	//DEBUG VALUES
	float xPrimeEnemy, yPrimeEnemy, angle;

public:
	SightComponent() {
		fieldOfView = 110;
	}

	SightComponent(float fov) {
		fieldOfView = fov;
	}


	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:   canSeePlayer
	--
	-- DATE:       October 18, 2018
	--
	-- DESIGNER:   Michael Goll
	--
	-- PROGRAMMER: Michael Goll
	--
	-- INTERFACE:  canSeePlayer(const TransformComponent * sTC, const TransformComponent * tTC, const float sightDistance)
	--
	-- RETURNS:    void
	--
	-- PARAMETER:  const TransformComponent * sTC - TransformComponent of the source character/object.
	--             const TransformComponent * tTC - TransformComponent of the target character/object.
	--             const float sightDistance      - The minimum distance at which the source can see the target.
	--
	-- NOTES:      Determines whether or not the source character/object can see its target character/object.
	--             "sightDistance" doubles as the distance in which the target must be away from the target before being
	--                 deemed 'unseeable' again.
	----------------------------------------------------------------------------------------------------------------------*/
	bool canSeePlayer(const TransformComponent * sTC, const TransformComponent * tTC, const float sightDistance) {
		Vector2D enemyPos = sTC->position;
		Vector2D playerPos = tTC->position;
		bool canSee = false, closeEnough = false;

		//Create vector between enemy and its sight point
		float eX = xPrimeEnemy - sTC->position.x;
		float eY = yPrimeEnemy - sTC->position.y;

		//Create vector between target and source
		float vX = tTC->position.x - sTC->position.x;
		float vY = tTC->position.y - sTC->position.y;

		//Normalize the vectors
		float ePLen = sqrt(eX * eX + eY * eY);
		float xPNorm = eX / ePLen;
		float yPNorm = eY / ePLen;

		float vLen = sqrt(vX * vX + vY * vY);
		float vNormX = vX / vLen;
		float vNormY = vY / vLen;

		//Dot that with the forward vector
		float dotProduct = (xPNorm * vNormX) + (yPNorm * vNormY);

		//find angle between them
		float angle = static_cast<float>(acos(dotProduct) * 180.00f / M_PI);

		if (angle <= PERIF_VISION / 2) {
			canSee = true;
		}

		if (abs(vX) <= sightDistance && abs(vY) <= sightDistance) {
			closeEnough = true;
		}

		return (canSee && closeEnough);
	}

	void calculateSightLine(const TransformComponent * sTC, const float sightDistance) {
		xPrimeEnemy = static_cast<float>(sTC->position.x + (sin(sTC->angle + M_PI / 2) * sightDistance));
		yPrimeEnemy = static_cast<float>(sTC->position.y + (cos(sTC->angle - M_PI / 2) * sightDistance));
	}

	//-------------------- DEBUG FUNCTIONS --------------------
	float getXE() {
		return xPrimeEnemy;
	}

	float getYE() {
		return yPrimeEnemy;
	}
	//------------------------------------------------------------
};