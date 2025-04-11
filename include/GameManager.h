#pragma once

constexpr auto FRAME_RATE = 500; // ms;
constexpr auto ESCAPE = 27;
constexpr auto BOARD_HEIGHT = 25;
constexpr auto BOARD_WIDTH = 80;

#include <vector>
#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include "Shell.h"
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

    std::vector<Shell*> shells;

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
    void drawGameObjects();
    void checkCollisions();
    void updateCooldowns();
    bool checkGameOver();
	void gameOver();
    void handlePlayerInput(Tank* player);
	void shoot(Tank* player);
    static bool isKeyPressed(int key);
    bool isInBoard(GameObject* object);

    //void update();
    //void checkCollisions(Shell& shell);
    //void checkGameOver();
    //void render();
    //void showGameOver();
    //void generateMines();
    //void generateWalls();

};
