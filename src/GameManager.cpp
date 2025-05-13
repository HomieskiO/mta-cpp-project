#include "GameManager.h"
#include <iostream>
#include <fstream> 
#include "IOUtils.h"
#include <conio.h>
#include <windows.h>
#include "Screen.h"

GameManager::GameManager(bool coloredGame, const std::string& screenFile) {
	this->coloredGame = coloredGame;
	this->screenFile = screenFile;
	isRunning = false;
	isPaused = false;
	player1Tanks = {};
	player2Tanks = {};
	shells = {};
	player1Score = 0;
	player2Score = 0;

	tankMovementCooldown = false;
}

void GameManager::startGame() {
	isRunning = true;
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
		// Wait a short time to let player see the initial board and end the round
		Sleep(1000);
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
		if (wall.getX() == x && wall.getY() == y) {
			return false;
		}
	}

	for (const Mine& mine : mines) {
		if (mine.getX() == x && mine.getY() == y) {
			return false;
		}
	}

	for (const Tank* tank : player1Tanks) {
		if (tank->getX() == x && tank->getY() == y) {
			return false;
		}
	}

	for (const Tank* tank : player2Tanks) {
		if (tank->getX() == x && tank->getY() == y) {
			return false;
		}
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
	while (std::getline(inFile, line) && y < BOARD_HEIGHT) {
		for (int x = 0; x < line.size() && x < BOARD_WIDTH; ++x) {
			char ch = line[x];
			switch (ch) {
			case '#': walls.push_back(Wall(x, y, wallColor)); break;
			case '@': mines.push_back(Mine(x, y, mineColor)); break;
			case '1': player1Tanks.push_back(new Tank(x, y, P1_CONTROLS, player1Color)); break;
			case '2': player2Tanks.push_back(new Tank(x, y, P2_CONTROLS, player2Color)); break;
				//case 'L': Not sure yet what should I do with this
			}
		}
		y++;
	}

	// Validate cannons for all tanks
	for (Tank* tank : player1Tanks) {
		validateTankCannon(tank);
	}

	for (Tank* tank : player2Tanks) {
		validateTankCannon(tank);
	}

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

void GameManager::drawGameInfo() { //TODO: This will need to be changed to support the size rules of excercise 2 and to be on L position
	gotoxy(0, BOARD_HEIGHT);

	std::cout << "P1 \tActive Tank: " << player1ActiveTank << "\tLives: " << player1Tanks.size() << "\tScore: " << player1Score << "\n";
	std::cout << "P2 \tActive Tank: " << player2ActiveTank << "\tLives: " << player2Tanks.size() << "\tScore: " << player2Score;
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

