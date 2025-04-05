#include "Tank.h"
#include "MovingObject.h"
#include "Cannon.h"
#include <iostream>

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
    if (movementState == MovementState::FORWARD) {
		std::cout << "Moving forward from (" << x << ", " << y << ") in direction " << static_cast<int>(direction) << std::endl;
    }
    else if (movementState == MovementState::BACKWARD) {
		std::cout << "Moving backward from (" << x << ", " << y << ") in direction " << static_cast<int>(direction) << std::endl;
    }
    else if (movementState == MovementState::STAY) {
		std::cout << "Staying at (" << x << ", " << y << ") in direction " << static_cast<int>(direction) << std::endl;
    }
}

void Tank::draw() const {
    GameObject::draw();
    cannon->draw();
}
