#include "Cannon.h"
#include "GameManager.h"
#include "IOUtils.h"
#include <iostream>

Cannon::Cannon(int tankX, int tankY, Direction tankDirection) : GameObject('|', tankX, tankY - 1) {
	this->tankX = tankX;
	this->tankY = tankY;
	this->tankDirection = tankDirection;
}

Cannon::Cannon(int tankX, int tankY, Direction tankDirection, int color) : GameObject('|', tankX, tankY - 1, color) {
	this->tankX = tankX;
	this->tankY = tankY;
	this->tankDirection = tankDirection;
}

void Cannon::rotateCannon(Direction tankDirection) {
	this->tankDirection = tankDirection;

	int prevX = x;
	int prevY = y;

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

	if (prevX != x || prevY != y) {
		gotoxy(prevX, prevY);
		std::cout << " ";
	}
}

void Cannon::alignWithTank(int tankX, int tankY) {
	this->tankX = tankX;
	this->tankY = tankY;
	rotateCannon(tankDirection);
	if (x == -1) {
		x = BOARD_WIDTH;
	}
	if (y == -1) {
		y = BOARD_HEIGHT;
	}
}

Cannon::~Cannon() {
	gotoxy(x, y);
	std::cout << " ";
}