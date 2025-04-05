#include "MovingObject.h"

MovingObject::MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState)
	: GameObject(symbol, x, y) {
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
