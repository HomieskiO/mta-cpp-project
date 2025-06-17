#pragma once

constexpr auto FRAME_RATE_MS = 150;
constexpr auto ESCAPE = 27;
constexpr auto BOARD_HEIGHT = 24;
constexpr auto BOARD_WIDTH = 80;

constexpr auto PLAYER_1_COLOR = 4;
constexpr auto PLAYER_2_COLOR = 1;
constexpr auto WALL_COLOR = 5;
constexpr auto MINE_COLOR = 3;
constexpr int PLAYER_1_ID = 1;
constexpr int PLAYER_2_ID = 2;

constexpr auto SCREENS_DIR = "../screens/";

constexpr auto SCREEN_WIN_SCORE = 100;

#include <vector>
#include <Windows.h>
#include <string>

#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include "Shell.h"
#include "Tank.h"
#include "Mine.h"
#include "Wall.h"
#include "Screen.h"
#include "ComputerPlayer.h"
#include "GameRecorder.h"

class GameManager {
protected:
    bool coloredGame;
    bool isRunning;
    bool isPaused;
    std::string screenFile;
    std::vector<Tank*> player1Tanks;
    std::vector<Tank*> player2Tanks;
    int player1ActiveTank;
    int player2ActiveTank;
    int player1Score;
    int player2Score;
    PlayerType player1Type;
    PlayerType player2Type;
    std::vector<Shell*> shells;
    std::vector<Mine> mines;
    std::vector<Wall> walls;
    int legendX;
    int legendY;
    bool tankMovementCooldown;
    bool shouldSaveSteps;
    GameRecorder gameRecorder;

	//chatGPT prompt to create templates for collision checking
    template<typename T1, typename T2>
    void checkCollisions(std::vector<T1*>& objects1, std::vector<T2*>& objects2) {
        for (auto& obj1 : objects1) {
            for (auto& obj2 : objects2) {
                if (obj1->collidesWith(obj2)) {
                    handleCollision(obj1, obj2);
                }
            }
        }
    }

    template<typename T1, typename T2>
    void checkCollisions(std::vector<T1*>& objects1, const std::vector<T2>& objects2) {
        for (auto& obj1 : objects1) {
            for (const auto& obj2 : objects2) {
                if (obj1->collidesWith(&obj2)) {
                    handleCollision(obj1, &obj2);
                }
            }
        }
    }
    void shoot(Tank* player);
    void ClearAllObjects();
    void checkCollisions();
    void checkShellsCollisions();
    void checkTanksCollisions();
    void checkShellTanksCollisions(Shell* shell, std::vector<Tank*>& playerTanks, int& activeTankIndex, bool& collided);
    void checkShellTankCollisions(Shell* shell, Tank* tank, bool& collided);
    void checkShellCannonsCollisions(Shell* shell, std::vector<Tank*>& playerTanks, bool& collided);
    void checkShellCannonCollisions(Shell* shell, Tank* tank, bool& collided);
    void checkShellShellsCollisions(Shell* shell, bool& collided);
    void checkShellWallsCollisions(Shell* shell, bool& collided);
    void checkTanksMinesCollisions(std::vector<Tank*>& playerTanks, int& activeTankIndex);
    void checkTankWallsCollisions(Tank* player);
    void checkTanksWallsCollisions(std::vector<Tank*> playerTanks);
    void removeDeadTanks(std::vector<Tank*>& playerTanks, int& activeTankIndex);
    void updateCooldowns();
    bool checkGameOver();
    void gameOver();
    void handlePlayerInput(std::vector<Tank*>& tanks, int& activeTankIndex, std::vector<Tank*>& opponentTanks);
    bool isInBoard(GameObject* object);
    void drawGameObjects();
    void drawGameInfo();
    void pauseGame();
    void resumeGame();
    bool initializeGameObjects(const std::string& filename);
    bool isValidCannonPosition(int x, int y);
    void validateTankCannon(Tank* tank);
    void validateAllTanks();
    void updateGame();
    void rotateTanksCannons(std::vector<Tank*> player);
	void gameLoop();
	void moveTanks(std::vector<Tank*> player);
    bool playLevel(const std::string& screenFile);


public:
    GameManager(bool coloredGame, const std::string& screenFile = "", PlayerType player1Type = PlayerType::HUMAN, PlayerType player2Type = PlayerType::HUMAN, bool shouldSaveSteps = false);
    void startGame();
    GameManager(const GameManager&) = default;
    GameManager& operator=(const GameManager&) = default;
    ~GameManager();
};
