#include "MovingObject.h"
#include "Shell.h"
#include "GameManager.h"

Shell::Shell(int x, int y, Direction tankDirection) : MovingObject(SHELL_SYMBOL, x, y, tankDirection, MovementState::FORWARD) {
}

Shell::Shell(int x, int y, Direction tankDirection, int color) : MovingObject(SHELL_SYMBOL, x, y, tankDirection, MovementState::FORWARD, color) {
}


void Shell::move() {
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
		y = y - velocity;
		break;
	case Direction::UPRIGHT:
		x = x + velocity;
		y = y - velocity;
		break;
	case Direction::UPLEFT:
		x = x - velocity;
		y = y - velocity;
		break;
	case Direction::LEFT:
		x = x - velocity;
		y = y;
		break;
	case Direction::DOWN:
		x = x;
		y = y + velocity;
		break;
	case Direction::DOWNRIGHT:
		x = x + velocity;
		y = y + velocity;
		break;
	case Direction::DOWNLEFT:
		x = x - velocity;
		y = y + velocity;
		break;
	case Direction::RIGHT:
		x = x + velocity;
		y = y;
		break;
	}
}