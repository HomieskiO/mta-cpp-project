#include "Tank.h"
#include "MovingObject.h"
#include "Cannon.h"
#include <iostream>
#include "GameManager.h"

Shell::Shell(int x, int y, int cannonX, int cannonY, Direction cannonDirection) : MovingObject(SHELL_SYMBOL, x, y, cannonDirection, MovementState::FORWARD) {
	this->cannonX = cannonX;
	this->cannonY = cannonY;
}


void Shell::move() {
	//implement
}