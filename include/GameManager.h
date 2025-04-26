#pragma once

constexpr auto FRAME_RATE_MS = 300;
constexpr auto ESCAPE = 27;
constexpr auto BOARD_HEIGHT = 24;
constexpr auto BOARD_WIDTH = 80;

constexpr auto PLAYER_1_COLOR = 4;
constexpr auto PLAYER_2_COLOR = 1;
constexpr auto WALL_COLOR = 5;
constexpr auto MINE_COLOR = 3;

#include <vector>
#include <Windows.h>

#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include "Shell.h"
#include "Tank.h"
#include "Mine.h"
#include "Wall.h"

class GameManager {
private:
    bool coloredGame;
    bool isRunning;
    bool isPaused;
    std::vector<Tank*> player1Tanks;
    int player1ActiveTank;
    std::vector<Tank*> player2Tanks;
    int player2ActiveTank;

    std::vector<Shell*> shells;
    std::vector<Mine> mines;
    std::vector<Wall> walls;
    
    bool tankMovementCooldown;

    void initializeGameObjects();
    void generateTanks();
    void gameLoop();
    void pauseGame();
    void resumeGame();
    void updateGame();
    void drawGameObjects();
    void updateCooldowns();
    bool checkGameOver();
    void gameOver();
    void handlePlayerInput(std::vector<Tank*>& playerTanks, int& activeTankIndex);
    void shoot(Tank* player);
    static bool isKeyPressed(int key);
    bool isInBoard(GameObject* object);
    void generateMines();
    void generateWalls();
    void moveTanks(std::vector<Tank*> player);
    void checkTankOnMine(Tank* player);
    void checkCollisions();
    void checkShellsCollisions();
    void checkTanksCollisions();
    void removeDeadTanks(std::vector<Tank*>& playerTanks, int& activeTankIndex);
    void checkShellTanksCollisions(Shell* shell, std::vector<Tank*>& playerTanks, int& activeTankIndex,  bool& collided);
    void checkShellTankCollisions(Shell* shell, Tank* tank, bool& collided);
    void checkShellCannonsCollisions(Shell* shell, std::vector<Tank*>& playerTanks, bool& collided);
    void checkShellCannonCollisions(Shell* shell, Tank* tank, bool& collided);
    void checkShellShellsCollisions(Shell* shell, bool& collided);
    void checkShellWallsCollisions(Shell* shell, bool& collided);
    void checkTanksMinesCollisions(std::vector <Tank*>& playerTanks, int& activeTankIndex);
    void checkTanksWallsCollisions(std::vector<Tank*> player);
    void checkTankWallsCollisions(Tank* player);
    void drawGameInfo();

public:
    GameManager(bool coloredGame);
    void startGame();
    ~GameManager();
};
