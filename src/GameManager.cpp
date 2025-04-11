#include "GameManager.h"
#include <iostream>
#include "IOUtils.h"
#include <conio.h>
#include <windows.h>

GameManager::GameManager() {
	isRunning = false;
	isPaused = false;
	player1 = nullptr;
	player2 = nullptr;
	shells = {};

	tankMovementCooldown = false;
}

void GameManager::startGame() {
	isRunning = true;
	initializeGameObjects();
	hideCursor();
	gameLoop();
}

void GameManager::initializeGameObjects() {
	generateTanks();
	//generateWalls();
	//generateMines();
}

void GameManager::generateTanks() {
	player1 = new Tank(10, 10, P1_CONTROLS, PLAYER_1_COLOR);
	player2 = new Tank(10, 5, P2_CONTROLS, PLAYER_2_COLOR);
}

void GameManager::gameLoop() {
	while (isRunning) {
		if (_kbhit()) {
			char input = _getch();
			if (input == ESCAPE) {
				pauseGame();
			}
		}
		if (!isRunning) {
			break;
		}
		if (!isPaused) {
			handlePlayerInput(player1);
			handlePlayerInput(player2);

			updateGame();
			drawGameObjects();
			if (checkGameOver()) {
				gameOver();
			}
		}
		Sleep(FRAME_RATE);
	}
}

bool GameManager::isKeyPressed(int keyCode) {
	return GetAsyncKeyState(keyCode) & 0x8000;
}

void GameManager::handlePlayerInput(Tank* player) {
	PlayerControls controls = player->getControls();
	if (isKeyPressed(controls.shoot)) {
		this->shoot(player);
	}
	if (isKeyPressed(controls.stay)) {
		player->setMovementState(MovementState::STAY);
		return;
	}
	if (isKeyPressed(controls.rightTrackForward) && isKeyPressed(controls.rightTrackBackward)) {
		return;
	}
	if (isKeyPressed(controls.leftTrackForward) && isKeyPressed(controls.leftTrackBackward)) {
		return;
	}
	if (isKeyPressed(controls.rightTrackForward) && isKeyPressed(controls.leftTrackForward)) {
		player->setMovementState(MovementState::FORWARD);
	}
	if (isKeyPressed(controls.rightTrackBackward) && isKeyPressed(controls.leftTrackBackward)) {
		player->setMovementState(MovementState::BACKWARD);
	}
	if (isKeyPressed(controls.rightTrackForward) && !isKeyPressed(controls.leftTrackForward) && !isKeyPressed(controls.leftTrackBackward)) {
		player->rotateCannon(-45);
	}
	if (isKeyPressed(controls.rightTrackBackward) && !isKeyPressed(controls.leftTrackForward) && !isKeyPressed(controls.leftTrackBackward)) {
		player->rotateCannon(45);
	}
	if (isKeyPressed(controls.leftTrackForward) && !isKeyPressed(controls.rightTrackForward) && !isKeyPressed(controls.rightTrackBackward)) {
		player->rotateCannon(45);
	}
	if (isKeyPressed(controls.leftTrackBackward) && !isKeyPressed(controls.rightTrackForward) && !isKeyPressed(controls.rightTrackBackward)) {
		player->rotateCannon(-45);
	}
	if (isKeyPressed(controls.leftTrackForward) && isKeyPressed(controls.rightTrackBackward)) {
		player->rotateCannon(90);
	}
	if (isKeyPressed(controls.leftTrackBackward) && isKeyPressed(controls.rightTrackForward)) {
		player->rotateCannon(-90);
	}
}

void GameManager::shoot(Tank* player) {
	if (player->canShoot()) {
		shells.push_back(new Shell(player->getCannonX(), player->getCannonY(), player->getDirection(), player->getColor()));
		player->setCooldown(SHOOT_COOLDOWN + 1);
	}
}

void GameManager::updateGame() {
	if (!tankMovementCooldown) {
		player1->move();
		player2->move();
	}

	for (auto it = shells.begin(); it != shells.end(); ) {
		Shell* shell = *it;
		shell->move();
		if (!isInBoard(shell)) {
			delete shell;
			it = shells.erase(it);
		}
		else {
			++it;
		}
	}

	checkCollisions();
	updateCooldowns();
}

void GameManager::checkCollisions() {
	for (auto it = shells.begin(); it != shells.end(); ) {
		Shell* shell = *it;
		bool collided = false;

		if (shell && shell->collidesWith(player1)) {
			player1->setState(false);
			collided = true;
		}
		else if (shell && shell->collidesWith(player2)) {
			player2->setState(false);
			collided = true;
		}

		// TODO fix shell colliding with cannon in a diagonal(?) moving tank
		else if (shell && player1->getCannon() && shell->collidesWith(player1->getCannon())) {
			player1->removeCannon();
			collided = true;
		}
		else if (shell && player2->getCannon() && shell->collidesWith(player2->getCannon())) {
			player2->removeCannon();
			collided = true;
		}

		// TODO add walls collisions

		if (collided) {
			delete shell;
			it = shells.erase(it);
		}
		else {
			++it;
		}
	}

}

void GameManager::updateCooldowns() {
	if (player1->getCooldown() > 0) {
		player1->setCooldown(player1->getCooldown() - 1);
	}
	if (player2->getCooldown() > 0) {
		player2->setCooldown(player2->getCooldown() - 1);
	}

	tankMovementCooldown = !tankMovementCooldown;
}

bool GameManager::checkGameOver() {
	return !player1->isAlive() || !player2->isAlive();
}

void GameManager::drawGameObjects() {
	clearScreen();

	player1->draw();
	player2->draw();

	for (auto shell : shells) {
		if (shell != nullptr) {
			shell->draw();
		}
	}
}

bool GameManager::isInBoard(GameObject* object) {
	return object->getX() >= 0 && object->getX() < BOARD_WIDTH && object->getY() >= 0 && object->getY() < BOARD_HEIGHT;
}

void GameManager::pauseGame() {
	isPaused = true;
	clearScreen();
	std::cout << "Game paused. Press ESC to resume or X to return to the main menu.\n";
	while (isPaused) {
		if (_kbhit()) {
			char input = tolower(_getch());
			if (input == ESCAPE) {
				resumeGame();
				break;
			}
			else if (input == 'x') {
				gameOver();
				break;
			}
		}
		Sleep(FRAME_RATE / 2);
	}
}

void GameManager::resumeGame() {
	isPaused = false;
	std::cout << "Game resumed.\n";
	Sleep(FRAME_RATE / 2);
}

void GameManager::gameOver() {
	isRunning = false;
	clearScreen();

	if (!player1->isAlive() && !player2->isAlive()) {
		std::cout << "Game tied.\n";
	}
	else if (!player1->isAlive()) { 
		std::cout << "Player 2 Wins!\n"; 
	}
	else if (!player2->isAlive()) { 
		std::cout << "Player 1 Wins!\n"; 
	}
	std::cout << "Game ended. Returning to main menu...\n";
	Sleep(2*FRAME_RATE);
}
