#include "Tank.h"
#include "MovingObject.h"
#include "Cannon.h"
#include <iostream>
#include "GameManager.h"

Tank::Tank(int x, int y, PlayerControls controls) : MovingObject(TANK_SYMBOL, x, y, Direction::UP, MovementState::STAY) {
	this->controls = controls;
    this->cannon = new Cannon(x, y, direction);
}

PlayerControls Tank::getControls() const {
	return controls;
}

void Tank::setControls(PlayerControls controls) {
	this->controls = controls;
}

void Tank::rotateCannon(int angle) {
    
    // Convert direction enum to index
    int dirIndex = static_cast<int>(direction);

    // Convert angle to steps (1 step = 45°)
    int steps = angle / 45;

    // Rotate and wrap around (mod 8)
    int newIndex = (dirIndex + steps + 8) % 8;

    direction = static_cast<Direction>(newIndex);
    cannon->rotateCannon(direction);

}

void Tank::move() {
	int velocity = 0;
	if (movementState == MovementState::FORWARD) {
		velocity = 1;
	} else if (movementState == MovementState::BACKWARD) {
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
	cannon->alignWithTank(x, y);
}

void Tank::draw() const {
    GameObject::draw();
    cannon->draw();
}
