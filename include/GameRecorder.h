#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream> 

enum class ActionType {
    ROTATE_LEFT_45,
    ROTATE_LEFT_90,
    ROTATE_RIGHT_45,
    ROTATE_RIGHT_90,
    STOP_ROTATE,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    SHOOT,
    STAY,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_DOWN,
    END_GAME
};


struct GameStep {
    int tick;
    int playerId;
    int tankId;
    ActionType action;
};

struct CollisionEvent {
    int tick;
    std::string whatWasHit;
    int x, y;
};

struct MineDeathEvent {
    int tick;
    int playerId;
    int tankId;
    int x, y;
};


class GameRecorder {
public:
    GameRecorder(bool shouldSaveSteps);
    int tick;
    bool shouldSaveSteps;
    std::vector<CollisionEvent> collisions;
    std::vector<MineDeathEvent> mineDeaths;
    void logAction(const int playerId, const int tankId, ActionType action);
    void saveToFile(const std::string& filename);
    std::vector<GameStep> loadGameStepsFromCSV(const std::string& filePath);
    std::string writeResultFile(const std::string& screenFileName, int player1Score, int player2Score);
    //void loadFromFile(const std::string& filename);
    //const std::vector<GameStep>& getLoadedSteps() const;

private:
    std::vector<GameStep> steps;
};
