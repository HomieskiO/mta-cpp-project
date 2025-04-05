#pragma once

#include "GameObject.h"
#include "MovingObject.h"

class Cannon : public GameObject {
private:
	int tankX;
	int tankY;
	Direction tankDirection;

public:
	Cannon(int tankX, int tankY, Direction tankDirection);
	void rotateCannon(Direction direction);
};