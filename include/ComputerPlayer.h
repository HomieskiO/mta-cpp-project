#pragma once
#include "Tank.h"
#include "Shell.h"
#include "Wall.h"
#include <vector>

enum class PlayerType {
    HUMAN,
    COMPUTER
};

constexpr int DANGER_DISTANCE = 20;    // Distance at which we consider a shell dangerous
constexpr int SHOOTING_RANGE = 10;     // Maximum distance to consider shooting

class ComputerPlayer : public Tank {
public:
    ComputerPlayer(int x, int y, int color);
    void makeMove(const std::vector<Shell*>& shells,
                 const std::vector<Tank*>& tanks,
                 const std::vector<Wall>& walls) override;

private:
    bool shouldMoveTank(const std::vector<Shell*>& shells);
    bool shouldShoot(const std::vector<Tank*>& tanks) override;
    bool isOpponentInRange(const std::vector<Tank*>& opponentTanks);
    void moveTankFromShell(const std::vector<Shell*>& shells);
    void aimAtTank(const std::vector<Tank*>& opponentTanks);
    bool isPathClear(const Tank* target, const std::vector<Wall>& walls);
    Shell* isCloseToShell(const std::vector<Shell*>& shells);
    struct PointDistance {
        int xDistance;
        int yDistance;
        int distance;
    };
    PointDistance calculateDistance(int x1, int y1, int x2, int y2) const;
}; 