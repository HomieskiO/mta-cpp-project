#pragma once

constexpr auto FRAME_RATE = 1000; // milliseconds;
constexpr auto ESCAPE = 27;
#include <vector>
#include "GameObject.h"
#include "Tank.h"
//#include "Mine.h"
//#include "Wall.h"
//#include "Shell.h"

class GameManager {
private:
    bool isRunning;
    bool isPaused;
    Tank* player1;
    Tank* player2;
    //std::vector<Mine> mines;
    //std::vector<Wall> walls;
    //std::vector<Shell> shells;

public:
    GameManager();
	void initializeGameObjects();
	void generateTanks();
    void startGame();
	void gameLoop();
	void pauseGame();
	void resumeGame();
	void updateGame();
	void gameOver();

    //void handleInput();
    //void update();
    //void checkCollisions(Shell& shell);
    //void checkGameOver();
    //void render();
    //void showGameOver();
    //void generateMines();
    //void generateWalls();

};
