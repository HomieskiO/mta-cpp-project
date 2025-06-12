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
    STAY
};


struct GameStep {
    int tick;
    int playerId;
    int tankId;
    ActionType action;
};


class GameRecorder {
public:
    int tick;
    void logAction(const int playerId, const int tankId, ActionType action);
    void saveToFile(const std::string& filename);
    //void loadFromFile(const std::string& filename);
    //const std::vector<GameStep>& getLoadedSteps() const;

private:
    std::vector<GameStep> steps;
};
