#include "GameManager.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

GameManager::GameManager() {
	isRunning = false;
	isPaused = false;
}

void GameManager::initializeGameObjects() {
	std::cout << "Initializing game objects...\n";
	//generateWalls();
 //   generateTanks();
	//generateMines();
}

void GameManager::startGame() {
	initializeGameObjects();
	isRunning = true;
	gameLoop();
}

void GameManager::pauseGame() {
	isPaused = true;
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
}

void GameManager::gameOver() {
	isRunning = false;
	std::cout << "Game ended. Returning to main menu...\n";
}

void GameManager::updateGame() {
	std::cout << "Updating game..." << std::endl;
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
			updateGame();
		}
		Sleep(FRAME_RATE);
	}
}
