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
	player1 = new Tank(10, 4);
	player2 = new Tank(10, 10);
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

void GameManager::updateGame() {
	clearScreen();

	if (player1->collidesWith(*player2)) {
		std::cout << "Collision detected! Game Over.\n";
		gameOver();
		return;
	}

	player1->draw();
	player2->draw();

	player1->setY(player1->getY()+1);
	player2->setY(player2->getY() - 1);
	//std::cout << "Updating game..." << std::endl;
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
	std::cout << "Game ended. Returning to main menu...\n";
	Sleep(FRAME_RATE);
}
