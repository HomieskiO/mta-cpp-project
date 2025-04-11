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
	Cannon(int tankX, int tankY, Direction tankDirection, int color);
	void rotateCannon(Direction direction);
	void alignWithTank(int tankX, int tankY);
};