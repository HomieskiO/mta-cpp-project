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

	player1 = new Tank(6, 3, P1_CONTROLS, player1Color);
	player2 = new Tank(72, 22, P2_CONTROLS, player2Color);
}

void GameManager::generateWalls() {
	int color;
	if (!coloredGame) {
		color = WHITE_COLOR;
	}
	else {
		color = WALL_COLOR;
	}
	// Central horizontal line
	for (int x = 20; x < 60; ++x) {
		walls.push_back(Wall(x, 12, color));
	}

	// Two vertical lanes near center
	for (int y = 5; y < 20; ++y) {
		walls.push_back(Wall(30, y, color));
		walls.push_back(Wall(50, y, color));
	}

	// Maze arms top left
	for (int x = 5; x < 20; ++x)
		walls.push_back(Wall(x, 4, color));
	for (int y = 4; y < 10; ++y)
		walls.push_back(Wall(5, y, color));

	// Maze arms bottom right
	for (int x = 60; x < 75; ++x)
		walls.push_back(Wall(x, 19, color));
	for (int y = 14; y < 20; ++y)
		walls.push_back(Wall(74, y, color));

	// Narrow passage top center
	for (int x = 35; x < 45; ++x)
		walls.push_back(Wall(x, 2, color));

	// Diagonal barrier (optional)
	for (int i = 0; i < 10; ++i)
		walls.push_back(Wall(10 + i, 14 + i, color));
}

void GameManager::generateMines() {
	int color;
	if (!coloredGame) {
		color = WHITE_COLOR;
	}
	else {
		color = MINE_COLOR;
	}
	// Mines scattered in open zones, away from spawn points
	mines.push_back(Mine(22, 8, color));
	mines.push_back(Mine(18, 18, color));
	mines.push_back(Mine(45, 5, color));
	mines.push_back(Mine(55, 15, color));
	mines.push_back(Mine(68, 10, color));
	mines.push_back(Mine(6, 21, color));
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

void GameManager::checkShellWallsCollisions(Shell* shell, bool& collided) {
	for (auto wallIt = walls.begin(); wallIt != walls.end(); ) {
		if (shell->collidesWith(*wallIt)) {
			wallIt->hit();
			collided = true;

			if (!wallIt->isAlive())
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

void GameManager::checkTanksMinesCollisions() {
	checkTankOnMine(player1);
	checkTankOnMine(player2);
}

//void GameManager::checkTanksWallsCollisions(Tank* player) {
//	for (auto wallIt = walls.begin(); wallIt != walls.end(); ) {
//		if (player->collidesWith(*wallIt)) {
//			player->setX(player->prevX);
//			player->setY(player->prevY);
//
//			// change also the cannon x y
//		}
//		++wallIt;
//	}
//}

void GameManager::checkCollisions() {
	checkShellsCollisions();
	checkTanksMinesCollisions();
	//checkTanksWallsCollisions(player1);
	//checkTanksWallsCollisions(player2);


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
	//clearScreen();

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
