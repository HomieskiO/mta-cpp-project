#include "GameManager.h"
#include <iostream>
#include "IOUtils.h"
#include <conio.h>
#include <windows.h>

GameManager::GameManager(bool coloredGame) {
	this->coloredGame = coloredGame;
	isRunning = false;
	isPaused = false;
	player1Tanks = {};
	player2Tanks = {};
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

	player1Tanks.push_back(new Tank(6, 6, P1_CONTROLS, player1Color));
	player1Tanks.push_back(new Tank(10, 3, P1_CONTROLS, player1Color));
	player2Tanks.push_back(new Tank(72, 22, P2_CONTROLS, player2Color));
	player2Tanks.push_back(new Tank(10, 7, P2_CONTROLS, player2Color));

	player1ActiveTank = 0;
	player2ActiveTank = 0;
}

void GameManager::generateWalls() {
	int color = WALL_COLOR;
	if (!coloredGame) {
		color = WHITE_COLOR;
	}

	for (int x = 20; x < 60; ++x) {
		walls.push_back(Wall(x, 12, color));
	}

	for (int y = 5; y < 20; ++y) {
		walls.push_back(Wall(30, y, color));
		walls.push_back(Wall(50, y, color));
	}

	for (int x = 5; x < 20; ++x)
		walls.push_back(Wall(x, 4, color));
	for (int y = 4; y < 10; ++y)
		walls.push_back(Wall(5, y, color));

	for (int x = 60; x < 75; ++x)
		walls.push_back(Wall(x, 19, color));
	for (int y = 14; y < 20; ++y)
		walls.push_back(Wall(74, y, color));

	for (int x = 35; x < 45; ++x)
		walls.push_back(Wall(x, 2, color));

	for (int i = 0; i < 10; ++i)
		walls.push_back(Wall(10 + i, 14 + i, color));
}

void GameManager::generateMines() {
	int color = MINE_COLOR;
	if (!coloredGame) {
		color = WHITE_COLOR;
	}

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
			if (!tankMovementCooldown) {
				handlePlayerInput(player1Tanks, player1ActiveTank);
				handlePlayerInput(player2Tanks, player2ActiveTank);
			}

			updateGame();
			drawGameObjects();
			drawGameInfo();
			if (checkGameOver()) {
				gameOver();
			}
		}
		Sleep(FRAME_RATE_MS);
	}
}

// GPT prompt - give me a function that gets keyboard inputs async
bool GameManager::isKeyPressed(int keyCode) {
	return GetAsyncKeyState(keyCode) & 0x8000;
}

void GameManager::handlePlayerInput(std::vector<Tank*>& playerTanks, int& activeTankIndex) {
	PlayerControls controls = playerTanks[activeTankIndex]->getControls();

	if (isKeyPressed(controls.switchActiveTank)) {
		activeTankIndex = (++activeTankIndex) % playerTanks.size();
	}

	Tank* player = playerTanks[activeTankIndex];
	
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

		// spawn shell one step further to prevent ruining your own cannon while moving
		shells.back()->move();
		player->setCooldown(SHOOT_COOLDOWN + 1);
	}
}

void GameManager::moveTanks(std::vector<Tank*> player) {
	for (auto& tank : player) {
		tank->move();
	}
}

void GameManager::updateGame() {
	if (!tankMovementCooldown) {
		moveTanks(player1Tanks);
		moveTanks(player2Tanks);
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

		checkShellTanksCollisions(shell, player1Tanks, player1ActiveTank, collided);
		checkShellTanksCollisions(shell, player2Tanks, player2ActiveTank, collided);
		checkShellCannonsCollisions(shell, player1Tanks, collided);
		checkShellCannonsCollisions(shell, player2Tanks, collided);
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

void GameManager::removeDeadTanks(std::vector<Tank*>& playerTanks, int& activeTankIndex) {
	int currentTankIndex = 0;
	for (auto it = playerTanks.begin(); it != playerTanks.end();) {
		Tank* tank = *it;
		if (!tank->isAlive()) {
			delete tank;
			it = playerTanks.erase(it);
			if (currentTankIndex < activeTankIndex) {
				activeTankIndex--;
			}
			else if (currentTankIndex == activeTankIndex && playerTanks.size()) {
				activeTankIndex = activeTankIndex % playerTanks.size();
			}

		} else{
			it++;
		}
		currentTankIndex++;
	}
}

void GameManager::checkShellTanksCollisions(Shell* shell, std::vector<Tank*>& playerTanks, int& activeTankIndex, bool& collided) {
	for (auto& tank : playerTanks) {
		checkShellTankCollisions(shell, tank, collided);
	}
	removeDeadTanks(playerTanks, activeTankIndex);
}

void GameManager::checkShellTankCollisions(Shell* shell, Tank* tank, bool& collided) {
	if (shell && shell->collidesWith(tank)) {
		tank->setState(false);
		collided = true;
	}
}

void GameManager::checkShellCannonsCollisions(Shell* shell, std::vector<Tank*>& playerTanks, bool& collided) {
	for (auto& tank : playerTanks) {
		checkShellCannonCollisions(shell, tank, collided);
	}
}

void GameManager::checkShellCannonCollisions(Shell* shell, Tank* tank, bool& collided) {
	if (shell && tank->getCannon() && shell->collidesWith(tank->getCannon())) {
		tank->removeCannon();
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

void GameManager::checkTanksMinesCollisions(std::vector <Tank*>& playerTanks, int& activeTankIndex) {
	for (auto& tank : playerTanks) {
		checkTankOnMine(tank);
	}
	removeDeadTanks(playerTanks, activeTankIndex);
}

void GameManager::checkTankWallsCollisions(Tank* player) {
	for (auto wallIt = walls.begin(); wallIt != walls.end(); ) {
		bool isTankHittingWall = player->collidesWith(*wallIt);
		bool isCannonHittingWall = player->getCannon() && player->getCannon()->collidesWith(*wallIt);
	
		if (player->getMovementState() == MovementState::STAY && isCannonHittingWall) {
			player->rotateCannon(-player->getLastRotation());
		}
		else if (player->getMovementState() != MovementState::STAY && (isTankHittingWall || isCannonHittingWall)) {
			player->setX(player->getPrevX());
			player->setY(player->getPrevY());
			player->alignCannon();
			isCannonHittingWall = player->getCannon() && player->getCannon()->collidesWith(*wallIt);
			if (isCannonHittingWall) {
				player->rotateCannon(-player->getLastRotation());
			}
			player->setMovementState(MovementState::STAY);
		}
		++wallIt;
	}
}

void GameManager::checkTanksWallsCollisions(std::vector<Tank*> playerTanks) {
	for (auto& tank : playerTanks) {
		checkTankWallsCollisions(tank);
	}
}

void GameManager::checkTanksCollisions() {
	checkTanksMinesCollisions(player1Tanks, player1ActiveTank);
	checkTanksMinesCollisions(player2Tanks, player2ActiveTank);
	checkTanksWallsCollisions(player1Tanks);
	checkTanksWallsCollisions(player2Tanks);
}

void GameManager::checkCollisions() {
	checkShellsCollisions();
	checkTanksCollisions();
}

void GameManager::checkTankOnMine(Tank* tank) {
	for (const Mine& mine : mines) {
		if (tank->collidesWith(mine)) {
			tank->setState(false);
		}
	}
}

void GameManager::updateCooldowns() {
	for (auto& tank : player1Tanks) {
		if (tank->getCooldown() > 0) {
			tank->setCooldown(tank->getCooldown() - 1);
		}
	}
	for (auto& tank : player2Tanks) {
		if (tank->getCooldown() > 0) {
			tank->setCooldown(tank->getCooldown() - 1);
		}
	}

	tankMovementCooldown = !tankMovementCooldown;
}

bool GameManager::checkGameOver() {
	return player1Tanks.empty() || player2Tanks.empty();
}

void GameManager::drawGameObjects() {
	for (const Wall& wall : walls) {
		wall.draw();
	}
	for (const Mine& mine : mines) {
		mine.draw();
	}

	for (auto& tank : player1Tanks) {
		tank->draw();
	}

	for (auto& tank : player2Tanks) {
		tank->draw();
	}

	for (auto shell : shells) {
		if (shell != nullptr) {
			shell->draw();
		}
	}
}

bool GameManager::isInBoard(GameObject* object) {
	return object->getX() >= 0 && object->getX() < BOARD_WIDTH && object->getY() >= 0 && object->getY() < BOARD_HEIGHT;
}

void GameManager::drawGameInfo() {
	gotoxy(0, BOARD_HEIGHT);
	
	std::cout << "Player 1 Active Tank: " << player1ActiveTank << "\t\tPlayer 1 Lives: " << player1Tanks.size() << "\t\t";
	std::cout << "Player 2 Active Tank: " << player2ActiveTank << "\t\tPlayer 2 Lives: " << player2Tanks.size();
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

	if (!player1Tanks.size() && !player2Tanks.size()) {
		std::cout << "Game tied.\n";
	}
	else if (!player1Tanks.size()) { 
		std::cout << "Player 2 Wins!\n"; 
	}
	else if (!player2Tanks.size()) { 
		std::cout << "Player 1 Wins!\n"; 
	}
	std::cout << "Game ended. Press any key to return to the main menu...\n";
	_getch();
}

GameManager::~GameManager() {
	for (auto& tank : player1Tanks) {
		delete tank;
	}

	for (auto& tank : player2Tanks) {
		delete tank;
	}
}
