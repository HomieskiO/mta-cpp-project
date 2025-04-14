#include "MovingObject.h"
#include "GameManager.h"
#include "IOUtils.h"
#include <iostream>

MovingObject::MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState)
	: GameObject(symbol, x, y) {
	this->direction = direction;
	this->movementState = movementState;
	this->prevX = x;
	this->prevY = y;
}

MovingObject::MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState, int color)
	: GameObject(symbol, x, y, color) {
	this->direction = direction;
	this->movementState = movementState;
	this->prevX = x;
	this->prevY = y;
}

MovementState MovingObject::getMovementState() const {
	return movementState;
}

void MovingObject::setMovementState(MovementState state) {
	movementState = state;
}

Direction MovingObject::getDirection() const {
	return direction;
}

void MovingObject::setDirection(Direction direction) {
	this->direction = direction;
}

void MovingObject::draw() const {
	gotoxy(x, y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	std::cout << symbol;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_COLOR);
}

void MovingObject::move() {
	prevX = x;
	prevY = y;

	int velocity = 0;
	if (movementState == MovementState::FORWARD) {
		velocity = 1;
	}
	else if (movementState == MovementState::BACKWARD) {
		velocity = -1;
	}

	switch (direction) {
	case Direction::UP:
		x = x;
		y = (BOARD_HEIGHT + y - velocity) % BOARD_HEIGHT;
		break;
	case Direction::UPRIGHT:
		x = (BOARD_WIDTH + x + velocity) % BOARD_WIDTH;
		y = (BOARD_HEIGHT + y - velocity) % BOARD_HEIGHT;
		break;
	case Direction::UPLEFT:
		x = (BOARD_WIDTH + x - velocity) % BOARD_WIDTH;
		y = (BOARD_HEIGHT + y - velocity) % BOARD_HEIGHT;
		break;
	case Direction::LEFT:
		x = (BOARD_WIDTH + x - velocity) % BOARD_WIDTH;
		y = y;
		break;
	case Direction::DOWN:
		x = x;
		y = (BOARD_HEIGHT + y + velocity) % BOARD_HEIGHT;
		break;
	case Direction::DOWNRIGHT:
		x = (BOARD_WIDTH + x + velocity) % BOARD_WIDTH;
		y = (BOARD_HEIGHT + y + velocity) % BOARD_HEIGHT;
		break;
	case Direction::DOWNLEFT:
		x = (BOARD_WIDTH + x - velocity) % BOARD_WIDTH;
		y = (BOARD_HEIGHT + y + velocity) % BOARD_HEIGHT;
		break;
	case Direction::RIGHT:
		x = (BOARD_WIDTH + x + velocity) % BOARD_WIDTH;
		y = y;
		break;
	}

	if (prevX != x || prevY != y) {
		gotoxy(prevX, prevY);
		std::cout << " ";
	}
}