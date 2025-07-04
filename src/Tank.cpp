#include "Tank.h"
#include "MovingObject.h"
#include "Cannon.h"
#include "Shell.h"
#include <iostream>
#include "IOUtils.h"
#include "GameManager.h"

Tank::Tank(int x, int y, PlayerControls controls) : MovingObject(TANK_SYMBOL, x, y, Direction::UP, MovementState::STAY) {
	this->controls = controls;
    this->cannon = new Cannon(x, y, direction);
	this->shell = nullptr;
	this->cooldown = 0;
	this->rotation = 0;
	this->lastRotation = 0;
}

Tank::Tank(int x, int y, PlayerControls controls, int color) : MovingObject(TANK_SYMBOL, x, y, Direction::UP, MovementState::STAY, color) {
	this->controls = controls;
	this->cannon = new Cannon(x, y, direction, color);
	this->shell = nullptr;
	this->cooldown = 0;
	this->rotation = 0;
	this->lastRotation = 0;
}

PlayerControls Tank::getControls() const {
	return controls;
}

void Tank::setControls(PlayerControls controls) {
	this->controls = controls;
}

void Tank::setRotation(int rotation) {
	this->rotation = rotation;
}
int Tank::getRotation() const {
	return rotation;
}

int Tank::getLastRotation() const {
	return lastRotation;
}

void Tank::rotateCannon() {
	Tank::rotateCannon(rotation);
}

void Tank::rotateCannon(int angle) { 
	lastRotation = angle;

    int dirIndex = static_cast<int>(direction);
    int steps = angle / 45;
    int newIndex = (dirIndex + steps + 8) % 8;

    MovingObject::setDirection(static_cast<Direction>(newIndex));
	if (cannon) {
		cannon->rotateCannon(direction);
	}
}

void Tank::move() {
	MovingObject::move();

	alignCannon();
}

void Tank::alignCannon() {
	if (cannon) {
		cannon->alignWithTank(x, y);
	}
}

bool Tank::canShoot() {
	return cannon != nullptr && cooldown == 0;
}

void Tank::draw() const {
    MovingObject::draw();
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

Tank::~Tank() {
	delete cannon;
	gotoxy(x, y);
	std::cout << " ";
}