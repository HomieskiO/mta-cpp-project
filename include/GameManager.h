#pragma once

constexpr auto FRAME_RATE = 500; // milliseconds;
constexpr auto ESCAPE = 27;
constexpr auto BOARD_HEIGHT = 25;
constexpr auto BOARD_WIDTH = 80;

#include <vector>
#include "GameObject.h"
#include "Tank.h"
#include <Windows.h>

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
    void handlePlayerInput(Tank* player);
    static bool isKeyPressed(int key);


    //void update();
    //void checkCollisions(Shell& shell);
    //void checkGameOver();
    //void render();
    //void showGameOver();
    //void generateMines();
    //void generateWalls();

};
