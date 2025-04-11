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
	shell = nullptr;
}

void GameManager::startGame() {
	isRunning = true;
	initializeGameObjects();
	hideCursor();
	gameLoop();
}

void GameManager::initializeGameObjects() {
	//generateWalls();
	generateTanks();
	//generateMines();
}

void GameManager::generateTanks() {
	player1 = new Tank(10, 10, P1_CONTROLS);
	player2 = new Tank(10, 5, P2_CONTROLS);
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
		shell = new Shell(player->getCannonX(), player->getCannonY(), player->getDirection());
		player->setCooldown(SHOOT_COOLDOWN + 1);
	}
}

void GameManager::updateGame() {
	player1->move();
	player2->move();

	// maybe move to updateShells
	if (shell != nullptr) {
		shell->move();
		if (!isInBoard(shell)) {
			delete shell;
			shell = nullptr;
		}
	}

	checkCollisions();
	updateCooldowns();
}

void GameManager::checkCollisions() {
	if (shell != nullptr && shell->collidesWith(player1)) {
		player1->setState(false);
		delete shell;
		shell = nullptr;
	}
	if (shell != nullptr && shell->collidesWith(player2)) {
		player2->setState(false);
		delete shell;
		shell = nullptr;
	}
}

void GameManager::updateCooldowns() {
	if (player1->getCooldown() > 0) {
		player1->setCooldown(player1->getCooldown() - 1);
	}
	if (player2->getCooldown() > 0) {
		player2->setCooldown(player2->getCooldown() - 1);
	}
}

bool GameManager::checkGameOver() {
	return !player1->isAlive() || !player2->isAlive();
}

void GameManager::drawGameObjects() {
	clearScreen();

	player1->draw();
	player2->draw();

	if (shell != nullptr){
		shell->draw();
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
		Sleep(FRAME_RATE / 10);
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
	std::cout << "Game ended. Returning to main menu...\n";
	Sleep(2*FRAME_RATE);
}
