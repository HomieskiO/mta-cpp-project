#include "GameManager.h"
#include <iostream>
#include "IOUtils.h"
#include <conio.h>
#include <windows.h>

GameManager::GameManager(bool coloredGame) {
	this->coloredGame = coloredGame;
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
	generateWalls();
	generateMines();
}

void GameManager::generateTanks() {
	int player1Color = PLAYER_1_COLOR;
	int player2Color = PLAYER_2_COLOR;

	if (!coloredGame) {
		player1Color = WHITE_COLOR;
		player2Color = WHITE_COLOR;
	}

	player1 = new Tank(10, 10, P1_CONTROLS, player1Color);
	player2 = new Tank(10, 4, P2_CONTROLS, player2Color);
}

void GameManager::generateWalls() {
	walls.push_back(Wall(5, 3, WALL_COLOR));
	walls.push_back(Wall(1, 2, WALL_COLOR));
	walls.push_back(Wall(7, 10, WALL_COLOR));
}

void GameManager::generateMines() {
	mines.push_back(Mine(3, 5, MINE_COLOR));
	mines.push_back(Mine(2, 1, MINE_COLOR));
	mines.push_back(Mine(10, 7, MINE_COLOR));
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
		Sleep(FRAME_RATE_MS);
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

void GameManager::checkShellsCollisions() {
	for (auto it = shells.begin(); it != shells.end(); ) {
		Shell* shell = *it;
		bool collided = false;

		checkShellTanksCollisions(shell, collided);
		checkShellCannonsCollisions(shell, collided);
		checkShellShellsCollisions(shell, collided);
		checkShellWallsCollisions(shell, collided);

		if (collided) {
			delete shell;
			it = shells.erase(it);
		}
		else {
			++it;
		}
	}
}

void GameManager::checkShellTanksCollisions(Shell* shell, bool& collided) {
	if (shell && shell->collidesWith(player1)) {
		player1->setState(false);
		collided = true;
	}
	else if (shell && shell->collidesWith(player2)) {
		player2->setState(false);
		collided = true;
	}
}

void GameManager::checkShellCannonsCollisions(Shell* shell, bool& collided) {
	if (shell && player1->getCannon() && shell->collidesWith(player1->getCannon())) {
		player1->removeCannon();
		collided = true;
	}
	else if (shell && player2->getCannon() && shell->collidesWith(player2->getCannon())) {
		player2->removeCannon();
		collided = true;
	}
}

void GameManager::checkShellShellsCollisions(Shell* shell, bool& collided) {
	for (auto it = shells.begin(); it != shells.end(); ) {
		Shell* otherShell = *it;

		if (otherShell == shell || otherShell == nullptr) {
			++it;
			continue;
		}

		if (shell->collidesWith(*otherShell)) {
			collided = true;
			delete otherShell;
			it = shells.erase(it);

			break;
		}
		else {
			++it;
		}
	}

}

void GameManager::checkTanksMinesCollisions() {
	checkTankOnMine(player1);
	checkTankOnMine(player2);
}

void GameManager::checkShellWallsCollisions(Shell* shell, bool& collided) {
	for (auto wallIt = walls.begin(); wallIt != walls.end(); ) {
		if (shell->collidesWith(*wallIt)) {
			wallIt->hit();
			collided = true;

			if (!wallIt->isAlive())
				// if changing walls to pinter add delete
				wallIt = walls.erase(wallIt);
			else {
				wallIt->setColor(WHITE_COLOR);
				++wallIt;
			}
			break;
		}
		else {
			++wallIt;
		}
	}
}

void GameManager::checkCollisions() {
	checkShellsCollisions();
	checkTanksMinesCollisions();

}

void GameManager::checkTankOnMine(Tank* player) {
	for (const Mine& mine : mines) {
		if (player->collidesWith(mine)) {
			player->setState(false);
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

	for (const Wall& wall : walls) {
		wall.draw();
	}
	for (const Mine& mine : mines) {
		mine.draw();
	}

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
		Sleep(FRAME_RATE_MS / 2);
	}
}

void GameManager::resumeGame() {
	isPaused = false;
	std::cout << "Game resumed.\n";
	Sleep(FRAME_RATE_MS / 2);
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
	std::cout << "Game ended. Press any key to return to the main menu...\n";
	_getch();
}
