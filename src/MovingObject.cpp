#include "MovingObject.h"
#include "GameManager.h"

MovingObject::MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState)
	: GameObject(symbol, x, y) {
	this->direction = direction;
	this->movementState = movementState;
}

MovingObject::MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState, int color)
	: GameObject(symbol, x, y, color) {
	this->direction = direction;
	this->movementState = movementState;
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

void MovingObject::getNextPosition(int& nextX, int& nextY) const {
    int velocity = 0;
    if (movementState == MovementState::FORWARD)
        velocity = 1;
    else if (movementState == MovementState::BACKWARD)
        velocity = -1;

    nextX = x;
    nextY = y;

    switch (direction) {
    case Direction::UP:
        nextY = (BOARD_HEIGHT + y - velocity) % BOARD_HEIGHT;
        break;
    case Direction::UPRIGHT:
        nextX = (BOARD_WIDTH + x + velocity) % BOARD_WIDTH;
        nextY = (BOARD_HEIGHT + y - velocity) % BOARD_HEIGHT;
        break;
    case Direction::UPLEFT:
        nextX = (BOARD_WIDTH + x - velocity) % BOARD_WIDTH;
        nextY = (BOARD_HEIGHT + y - velocity) % BOARD_HEIGHT;
        break;
    case Direction::LEFT:
        nextX = (BOARD_WIDTH + x - velocity) % BOARD_WIDTH;
        break;
    case Direction::DOWN:
        nextY = (BOARD_HEIGHT + y + velocity) % BOARD_HEIGHT;
        break;
    case Direction::DOWNRIGHT:
        nextX = (BOARD_WIDTH + x + velocity) % BOARD_WIDTH;
        nextY = (BOARD_HEIGHT + y + velocity) % BOARD_HEIGHT;
        break;
    case Direction::DOWNLEFT:
        nextX = (BOARD_WIDTH + x - velocity) % BOARD_WIDTH;
        nextY = (BOARD_HEIGHT + y + velocity) % BOARD_HEIGHT;
        break;
    case Direction::RIGHT:
        nextX = (BOARD_WIDTH + x + velocity) % BOARD_WIDTH;
        break;
    }
}

void MovingObject::move() {
    int nextX, nextY;
    getNextPosition(nextX, nextY);

    x = nextX;
    y = nextY;
}

void MovingObject::moveToPoint(int nextX, int nextY) {
    x = nextX;
    y = nextY;
}