#pragma once

constexpr auto FRAME_RATE = 200; // ms;
constexpr auto ESCAPE = 27;
constexpr auto BOARD_HEIGHT = 25;
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
    bool isRunning;
    bool isPaused;
    Tank* player1;
    Tank* player2;

    std::vector<Shell*> shells;
    std::vector<Mine> mines;
    std::vector<Wall> walls;
    
    bool tankMovementCooldown;

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
    void generateMines();
    void generateWalls();

    //void update();
    //void checkCollisions(Shell& shell);
    //void checkGameOver();
    //void render();
    //void showGameOver();


};
