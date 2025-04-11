#include "Tank.h"
#include "MovingObject.h"
#include "Cannon.h"
#include "Shell.h"
#include <iostream>
#include "GameManager.h"

Tank::Tank(int x, int y, PlayerControls controls) : MovingObject(TANK_SYMBOL, x, y, Direction::UP, MovementState::STAY) {
	this->controls = controls;
    this->cannon = new Cannon(x, y, direction);
	this->shell = nullptr;
	this->cooldown = 0;
}

Tank::Tank(int x, int y, PlayerControls controls, int color) : MovingObject(TANK_SYMBOL, x, y, Direction::UP, MovementState::STAY, color) {
	this->controls = controls;
	this->cannon = new Cannon(x, y, direction, color);
	this->shell = nullptr;
	this->cooldown = 0;
}

PlayerControls Tank::getControls() const {
	return controls;
}

void Tank::setControls(PlayerControls controls) {
	this->controls = controls;
}

void Tank::rotateCannon(int angle) {   
    int dirIndex = static_cast<int>(direction);
    int steps = angle / 45;
    int newIndex = (dirIndex + steps + 8) % 8;

    direction = static_cast<Direction>(newIndex);
    cannon->rotateCannon(direction);
}

void Tank::move() {
	// TODO prevent moving on a wall after adding walls

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
	if (cannon) {
		cannon->alignWithTank(x, y);
	}
}

bool Tank::canShoot() {
	return cannon != nullptr && cooldown == 0;
}

void Tank::draw() const {
    GameObject::draw();
	if (cannon) {
		cannon->draw();
	}
}

int Tank::getCannonX() const {
	return cannon->getX();
}

int Tank::getCannonY() const {
	return cannon->getY();
}

int Tank::getCooldown() {
	return cooldown;
}

void Tank::setCooldown(int cooldown) {
	this->cooldown = cooldown;
}

Cannon* Tank::getCannon() {
	return cannon;
}

void Tank::removeCannon() {
	delete cannon;
	cannon = nullptr;
}

int Tank::getColor() {
	return color;
}