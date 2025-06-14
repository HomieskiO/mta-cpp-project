#include "GameObject.h"
#include "MovingObject.h"
#include "Tank.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "GameManager.h"
#include <iostream>
#include <fstream> 
#include "IOUtils.h"
#include <conio.h>
#include <windows.h>
#include "Screen.h"

GameManager::GameManager(bool coloredGame, const std::string& screenFile, PlayerType player1Type, PlayerType player2Type) {
	this->coloredGame = coloredGame;
	this->screenFile = screenFile;
	this->player1Type = player1Type;
	this->player2Type = player2Type;
	isRunning = false;
	isPaused = false;
	player1Tanks = {};
	player2Tanks = {};
	shells = {};
	player1Score = 0;
	player2Score = 0;
	legendX = 0; 
	legendY = 0; 
	tankMovementCooldown = false;
}

void GameManager::startGame() {
	isRunning = true;
	GameRecorder gameRecorder;
	gameRecorder.tick = 0;
	clearScreen();
	ClearAllObjects();

	// If no screen file was chosen, get the first screen in lexicographic order
	if (screenFile.empty()) {
		std::vector<Screen> screens;
		if (Screen::loadAllScreenFiles(screens) && !screens.empty()) {
			screenFile = SCREENS_DIR + screens[0].name;
		}
		else {
			std::cerr << "No screen files found. Cannot start game.\n";
		}
	}

	if (!initializeGameObjects(screenFile)) {
		std::cerr << "Failed to initialize game objects from screen file: " << screenFile << "\n";
	}

	hideCursor();
	if (player1Tanks.empty() || player2Tanks.empty()) {
		drawGameObjects();
		drawGameInfo();
		Sleep(1000); // Wait a short time to let player see the initial board and end the round
		gameOver();
		return;
	}

	gameLoop();
}

void GameManager::ClearAllObjects() {
	for (auto& tank : player1Tanks) {
		delete tank;
	}
	for (auto& tank : player2Tanks) {
		delete tank;
	}
	for (auto& shell : shells) {
		delete shell;
	}
	player1Tanks.clear();
	player2Tanks.clear();
	shells.clear();
	mines.clear();
	walls.clear();
}

bool GameManager::isValidCannonPosition(int x, int y) {
	if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
		return false;
	}
	for (const Wall& wall : walls) {
		if (wall.getX() == x && wall.getY() == y) return false;
	}
	for (const Mine& mine : mines) {
		if (mine.getX() == x && mine.getY() == y) return false;
	}
	for (const Tank* tank : player1Tanks) {
		if (tank->getX() == x && tank->getY() == y) return false;
	}
	for (const Tank* tank : player2Tanks) {
		if (tank->getX() == x && tank->getY() == y) return false;
	}

	return true;
}

void GameManager::validateTankCannon(Tank* tank) {
	for (int i = 0; i < 8; i++) {
		if (isValidCannonPosition(tank->getCannonX(), tank->getCannonY())) {
			return;
		}
		tank->rotateCannon(45);
	}
	tank->removeCannon();
}

void GameManager::validateAllTanks() {
	for (Tank* tank : player1Tanks) {
		validateTankCannon(tank);
	}
	for (Tank* tank : player2Tanks) {
		validateTankCannon(tank);
	}
}

bool GameManager::initializeGameObjects(const std::string& filename) {
	clearScreen();
	std::ifstream inFile(filename);
	if (!inFile) {
		std::cerr << "Failed to open screen file: " << filename << "\n";
		return false;
	}

	int player1Color = coloredGame ? PLAYER_1_COLOR : WHITE_COLOR;
	int player2Color = coloredGame ? PLAYER_2_COLOR : WHITE_COLOR;
	int wallColor = coloredGame ? WALL_COLOR : WHITE_COLOR;
	int mineColor = coloredGame ? MINE_COLOR : WHITE_COLOR;

	std::string line;
	int y = 0;

	int p1_tank_id = 0;
	int p2_tank_id = 0;

	while (std::getline(inFile, line) && y < BOARD_HEIGHT) {
		for (size_t x = 0; x < line.size() && x < BOARD_WIDTH; ++x) {
			char ch = line[x];
			switch (ch) {
			case '#': walls.push_back(Wall(x, y, wallColor)); break;
			case '@': mines.push_back(Mine(x, y, mineColor)); break;
			case '1':
				if (player1Type == PlayerType::COMPUTER) {
					player1Tanks.push_back(new ComputerPlayer(x, y, player1Color));
				} else {
					player1Tanks.push_back(new HumanPlayer(x, y, P1_CONTROLS, player1Color, 1, p1_tank_id));
				}
				p1_tank_id++;
				break;
			case '2': 
				if (player2Type == PlayerType::COMPUTER) {
					player2Tanks.push_back(new ComputerPlayer(x, y, player2Color));
				} else {
					player2Tanks.push_back(new HumanPlayer(x, y, P2_CONTROLS, player2Color, 2, p2_tank_id));
				}
				p2_tank_id++;
				break;
			case 'L': legendX = x; legendY = y; break;
			}
		}
		y++;
	}

	validateAllTanks();
	return true;
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
				handlePlayerInput(player1Tanks, player1ActiveTank, player2Tanks);
				handlePlayerInput(player2Tanks, player2ActiveTank, player1Tanks);
			}

			updateGame();
			drawGameInfo();
			drawGameObjects();
			if (checkGameOver()) {
				gameOver();
			}
		}
		Sleep(FRAME_RATE_MS);
		gameRecorder.tick++;
	}
}

void GameManager::handlePlayerInput(std::vector<Tank*>& tanks, int& activeTankIndex, std::vector<Tank*>& opponentTanks) {
	//handle switching active tank for human type player before making noves
	PlayerControls controls = tanks[activeTankIndex]->getControls();
	if (isKeyPressed(controls.switchActiveTank)) {
		activeTankIndex = (++activeTankIndex) % tanks.size();
	}
	Tank* tank = tanks[activeTankIndex];
	tank->makeMove(shells, opponentTanks, walls, gameRecorder);
	if (tank->shouldShoot(opponentTanks)) {
		shoot(tank);
		gameRecorder.logAction(tank->playerId, tank->tankId, ActionType::SHOOT);
	}
}

void GameManager::shoot(Tank* player) {
	if (player->canShoot()) {
		shells.push_back(new Shell(player->getCannonX(), player->getCannonY(), player->getDirection(), player->getColor()));
		// spawn shell one step further to prevent ruining your own cannon while moving
		shells.back()->move();
		this->checkShellsCollisions();
		player->setCooldown(SHOOT_COOLDOWN + 1);
		Beep(500, 300); // Beep sound for shooting
	}
}

void GameManager::rotateTanksCannons(std::vector<Tank*> player) {
	for (auto& tank : player) {
		if (std::abs(tank->getRotation()) == 90) {
			tank->rotateCannon(tank->getRotation() / 2);
			checkTanksWallsCollisions(player);
			tank->rotateCannon(tank->getRotation() / 2);
		}
		else {
			tank->rotateCannon();
		}
	}
}

void GameManager::moveTanks(std::vector<Tank*> player) {
	for (auto& tank : player) {
		tank->move();
	}
}

void GameManager::updateGame() {
	if (!tankMovementCooldown) {
		rotateTanksCannons(player1Tanks);
		rotateTanksCannons(player2Tanks);
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
			Beep(3000, 300); // Beep sound for collision
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

		}
		else {
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

void GameManager::checkTanksMinesCollisions(std::vector<Tank*>& playerTanks, int& activeTankIndex) {
	for (auto& tank : playerTanks) {
		for (auto mineIt = mines.begin(); mineIt != mines.end();) {
			if (tank->collidesWith(*mineIt)) {
				tank->setState(false);
				mineIt = mines.erase(mineIt);
			}
			else {
				++mineIt;
			}
		}
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
			player->setRotation(0);
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
	gotoxy(legendX, legendY); 
	std::cout << "P1 | Active:" << player1ActiveTank << " Score:" << player1Score;
	gotoxy(legendX, legendY + 1);
	std::cout << "P2 | Active:" << player2ActiveTank << " Score:" << player2Score;
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
	isPaused = false;
	clearScreen();
	std::string message = "Level Ended";
	if (!player1Tanks.size() && !player2Tanks.size()) {
		// Game tied, no points awarded
		message = "Level tied!";
	}
	else if (!player1Tanks.size()) {
		player2Score += SCREEN_WIN_SCORE;
		message = "Player 2 wins this level!";
	}
	else if (!player2Tanks.size()) {
		player1Score += SCREEN_WIN_SCORE;
		message = "Player 1 wins this level!";
	}
	gameRecorder.saveToFile("test.record");
	// Load next screen if available
	std::vector<Screen> screens;
	if (Screen::loadAllScreenFiles(screens) && !screens.empty()) {
		size_t currentIndex = 0;
		for (size_t i = 0; i < screens.size(); i++) {
			if (SCREENS_DIR + screens[i].name == screenFile) {
				currentIndex = i;
				break;
			}
		}

		// Try to load next screen
		if (currentIndex + 1 < screens.size()) {
			screenFile = SCREENS_DIR + screens[currentIndex + 1].name;
			clearScreen();
			std::cout << "\t==========================================\n\n";
			std::cout << "\t\t" << message << "\n\n";
			std::cout << "\t==========================================\n\n";
			std::cout << "\tloading next screen " << screens[currentIndex + 1].name << "\n";
			std::cout << "\tPress any key to continue...\n";
			_getch();
			startGame();
			return;
		}
	}

	// Only show scores at the end of the game
	clearScreen();
	std::cout << "\n\t==========================================\n";
	std::cout << "\t              FINAL RESULTS               \n";
	std::cout << "\t==========================================\n";
	std::cout << "\tPlayer 1: " << player1Score << "\n";
	std::cout << "\tPlayer 2: " << player2Score << "\n\n";

	if (player1Score > player2Score) {
		std::cout << "\t\tPlayer 1 Wins the Game!\n";
	}
	else if (player2Score > player1Score) {
		std::cout << "\t\tPlayer 2 Wins the Game!\n";
	}
	else {
		std::cout << "\t\tThe Game is Tied!\n";
	}

	std::cout << "\n\t==========================================\n";
	std::cout << "\tPress any key to return to the main menu...\n";
	std::cout << "\t==========================================\n";
	_getch();
}

GameManager::~GameManager() {
	ClearAllObjects();
}

