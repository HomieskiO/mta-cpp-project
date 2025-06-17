#pragma once
#include "Tank.h"
#include "Shell.h"
#include "Wall.h"
#include "GameRecorder.h"
#include <vector>

constexpr int DANGER_DISTANCE = 20;    // Maximum distance at which we consider a shell dangerous

class GameManager;

class ComputerPlayer : public Tank {
    friend class GameManager;
private:
    ComputerPlayer(int x, int y, int color, int tankId, int playerId);
    void makeMove(const std::vector<Shell*>& shells,
        const std::vector<Tank*>& tanks,
        const std::vector<Wall>& walls,
        GameRecorder& gameRecorder) override;
    bool shouldShoot(const std::vector<Tank*>& opponentTanks) override;
    bool isShellChasing(const std::vector<Shell*>& shells) const;
    void moveAwayFromShell(const Shell* shell, GameRecorder& gameRecorder);
    bool canShootTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells) const;
    void aimAtTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells, GameRecorder& gameRecorder);
    bool isMyTanksInDirection(const std::vector<Tank*>& myTanks, Direction shootDir) const;
    bool isShellAimedAtMe(const Shell* shell) const;
    bool isInLineWithTarget(const GameObject* target) const;
    void setDirectionToTarget(const GameObject* target);
    std::vector<Shell*> currentShells; // Store current shells for shouldShoot
};