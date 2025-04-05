#include "Tank.h"
#include <iostream>

Tank::Tank(int x, int y, PlayerControls controls) : GameObject(TANK_SYMBOL, x, y, Direction::UP) {
	this->controls = controls;
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

    std::cout << "Rotating cannon from " << dirIndex << " to " << newIndex << std::endl;

    direction = static_cast<Direction>(newIndex);

}
