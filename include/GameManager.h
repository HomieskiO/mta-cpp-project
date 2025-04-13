#pragma once

constexpr auto FRAME_RATE_MS = 300;
constexpr auto ESCAPE = 27;
constexpr auto BOARD_HEIGHT = 24;
constexpr auto BOARD_WIDTH = 80;

constexpr auto PLAYER_1_COLOR = 4;
constexpr auto PLAYER_2_COLOR = 1;

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
    bool coloredGame;
    bool isRunning;
    bool isPaused;
    Tank* player1;
    Tank* player2;

    std::vector<Shell*> shells;

    //std::vector<Mine> mines;
    //std::vector<Wall> walls;
    
    bool tankMovementCooldown;

    void initializeGameObjects();
    void generateTanks();
    void gameLoop();
    void pauseGame();
    void resumeGame();
    void updateGame();
    void drawGameObjects();
    void checkCollisions();
    void checkShellTanksCollisions(Shell* shell, bool& collided);
    void checkShellCannonsCollisions(Shell* shell, bool& collided);
    void checkShellShellsCollisions(Shell* shell, bool& collided);
    void updateCooldowns();
    bool checkGameOver();
    void gameOver();
    void handlePlayerInput(Tank* player);
    void shoot(Tank* player);
    static bool isKeyPressed(int key);
    bool isInBoard(GameObject* object);
    
    //void generateMines();
    //void generateWalls();

public:
    GameManager(bool coloredGame);
    void startGame();
};
