#pragma once
#include "Tank.h"
#include "Wall.h"
#include "GameRecorder.h"
#include <vector>

class HumanPlayer : public Tank {
    friend class GameManager; 
public:
    HumanPlayer(int x, int y, const PlayerControls& controls, int color);
    HumanPlayer(int x, int y, const PlayerControls& controls, int color, int playerId, int tankId);
    void makeMove(const std::vector<Shell*>& shells,
                 const std::vector<Tank*>& tanks,
                 const std::vector<Wall>& walls,
                 GameRecorder& gameRecorder) override;
    bool shouldShoot(const std::vector<Tank*>& tanks) override;
}; 