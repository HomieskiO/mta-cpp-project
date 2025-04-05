#include "Cannon.h"

Cannon::Cannon(int tankX, int tankY, Direction tankDirection) : GameObject('|', tankX, tankY - 1) {
	this->tankX = tankX;
	this->tankY = tankY;
	this->tankDirection = tankDirection;
}

void Cannon::rotateCannon(Direction tankDirection) {
	this->tankDirection = tankDirection;

	switch (tankDirection) {
	case Direction::UP:
		this->x = tankX;
		this->y = tankY - 1;
		this->symbol = '|';
		break;
	case Direction::UPRIGHT:
		this->x = tankX + 1;
		this->y = tankY - 1;
		this->symbol = '/';
		break;
	case Direction::UPLEFT:
		this->x = tankX - 1;
		this->y = tankY - 1;
		this->symbol = '\\';
		break;
	case Direction::LEFT:
		this->x = tankX - 1;
		this->y = tankY;
		this->symbol = '-';
		break;
	case Direction::DOWN:
		this->x = tankX;
		this->y = tankY + 1;
		this->symbol = '|';
		break;
	case Direction::DOWNRIGHT:
		this->x = tankX + 1;
		this->y = tankY + 1;
		this->symbol = '\\';
		break;
	case Direction::DOWNLEFT:
		this->x = tankX - 1;
		this->y = tankY + 1;
		this->symbol = '/';
		break;
	case Direction::RIGHT:
		this->x = tankX + 1;
		this->y = tankY;
		this->symbol = '-';
		break;
	}
}