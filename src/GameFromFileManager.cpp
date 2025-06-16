#include "GameFromFileManager.h"

GameFromFileManager::GameFromFileManager(const std::string& screenFile, const std::string& stepsFile, bool isSilent)
	: GameManager(true, screenFile, PlayerType::HUMAN, PlayerType::HUMAN, false){
	this->isSilent = isSilent;
	this->stepsFile = stepsFile;
}

void GameFromFileManager::startGame() {
	isRunning = true;
	if (!initializeGameObjects(screenFile)) {
		std::cerr << "Failed to initialize game objects from screen file: " << screenFile << "\n";
	}
	this->steps = gameRecorder.loadGameStepsFromCSV(this->stepsFile);
	hideCursor();
	if (player1Tanks.empty() || player2Tanks.empty()) {
		drawGameObjects();
		drawGameInfo();
		Sleep(1000);
		gameOver();
		return;
	}

	gameLoop();
}

Tank* GameFromFileManager::findTankById(int playerId, int tankId) {
	std::vector<Tank*>& tanks = (playerId == 1) ? player1Tanks : player2Tanks;


	for (Tank* tank : tanks) {
		if (tank->tankId == tankId) {
			return tank;
		}
	}
	return nullptr;
}


void GameFromFileManager::applyGameStep(const GameStep& step) {
	if (step.action != ActionType::END_GAME) {
		Tank* tank = findTankById(step.playerId, step.tankId);
		if (!tank) return;
		switch (step.action) {
		case ActionType::ROTATE_LEFT_45:
			tank->setRotation(-45);
			break;
		case ActionType::ROTATE_LEFT_90:
			tank->setRotation(-90);
			break;
		case ActionType::ROTATE_RIGHT_45:
			tank->setRotation(45);
			break;
		case ActionType::ROTATE_RIGHT_90:
			tank->setRotation(90);
			break;
		case ActionType::STOP_ROTATE:
			tank->setRotation(0);
			break;
		case ActionType::MOVE_FORWARD:
			tank->setMovementState(MovementState::FORWARD);
			break;
		case ActionType::MOVE_BACKWARD:
			tank->setMovementState(MovementState::BACKWARD);
			break;
		case ActionType::SHOOT:
			this->shoot(tank);
			break;
		case ActionType::STAY:
			tank->setMovementState(MovementState::STAY);
			break;
		default:
			break;
		}
	}
}


void GameFromFileManager::gameLoop() {
	int currentStepIndex = 0;
	while (isRunning) {
		if (!isRunning) {
			break;
		}

		while (currentStepIndex < steps.size() &&
			steps[currentStepIndex].tick == gameRecorder.tick) {

			const GameStep& step = steps[currentStepIndex];

			applyGameStep(step);
			currentStepIndex++;
		}

		updateGame();
		if (!isSilent) {
			drawGameInfo();
			drawGameObjects();
		}
		if (currentStepIndex >= steps.size()) {
			if (checkGameOver()){
				gameOver();
				break;
			}
		}
		Sleep(FRAME_RATE_MS);
		gameRecorder.tick++;
	}
}

void GameFromFileManager::gameOver() {
	std::string resultFilePath = gameRecorder.writeResultFile(screenFile, player1Score, player2Score);
	clearScreen();
	std::ifstream resultFile(resultFilePath);
	if (resultFile.is_open()) {
		std::string line;
		while (std::getline(resultFile, line)) {
			std::cout << line << "\n";
		}
		resultFile.close();
	}
	else {
		std::cerr << "Could not open result file: " << resultFilePath << "\n";
	}

	std::cout << "\nPress any key to continue...";
}