#include "ComputerPlayer.h"
#include "GameManager.h"
#include <algorithm>
#include <cmath>

// General comment : computer player class  only aims and rotates, all shooting and movement is actually done in GameManager

ComputerPlayer::ComputerPlayer(int x, int y, int color)
    : Tank(x, y, P2_CONTROLS, color) {  // Using P2_CONTROLS as default value
}
// Only shoot if we can shoot and have a valid target  
bool ComputerPlayer::shouldShoot(const std::vector<Tank*>& opponentTanks) {
    return canShoot() && canShootTarget(opponentTanks, currentShells);
}

void ComputerPlayer::makeMove(const std::vector<Shell*>& shells,
    const std::vector<Tank*>& opponentTanks,
    const std::vector<Wall>& walls,
    GameRecorder& gameRecorder) {
    currentShells = shells;
    // First priority: move away from shells
    if (isShellChasing(shells)) {
        for (const auto& shell : shells) {
            if (std::abs(shell->getX() - getX()) <= DANGER_DISTANCE &&
                std::abs(shell->getY() - getY()) <= DANGER_DISTANCE) {
                moveAwayFromShell(shell);
                return;
            }
        }
    }
    else {
        setMovementState(MovementState::STAY);
    }
    // Second priority: Shoot targets if possible
    if (canShootTarget(opponentTanks, shells)) {
        aimAtTarget(opponentTanks, shells);
        return;
    }

    // Third priority will be moving towards opponent
    // TODO: Making it moving forward to prevent game from freezing but need to implement better logic and avoidance of mines and walls
    setMovementState(MovementState::FORWARD);
}

bool ComputerPlayer::isShellChasing(const std::vector<Shell*>& shells) const {
    for (const auto& shell : shells) {
        if (std::abs(shell->getX() - getX()) <= DANGER_DISTANCE &&
            std::abs(shell->getY() - getY()) <= DANGER_DISTANCE) {
            return true;
        }
    }
    return false;
}

void ComputerPlayer::moveAwayFromShell(const Shell* shell) {
    int dx = getX() - shell->getX();
    int dy = getY() - shell->getY();

    bool moveHorizontally = std::abs(dx) > std::abs(dy);
    Direction newDirection = moveHorizontally ?
        (dx > 0 ? Direction::RIGHT : Direction::LEFT) :
        (dy > 0 ? Direction::DOWN : Direction::UP);
    setDirection(newDirection);

    bool shellMovingTowards = moveHorizontally ?
        ((dx > 0 && shell->getDirection() == Direction::LEFT) ||
            (dx < 0 && shell->getDirection() == Direction::RIGHT)) :
        ((dy > 0 && shell->getDirection() == Direction::UP) ||
            (dy < 0 && shell->getDirection() == Direction::DOWN));

    setMovementState(shellMovingTowards ? MovementState::BACKWARD : MovementState::FORWARD);
}

bool ComputerPlayer::canShootTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells) const {
    // Check for shells aimed at us
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell) && isInLineWithTarget(shell)) {
            return true;
        }
    }

    // Check for opponent tanks to shoot at
    for (const auto& opponent : opponentTanks) {
        if (isInLineWithTarget(opponent)) {
            return true;
        }
    }
    return false;
}

void ComputerPlayer::aimAtTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells) {
    // First try to aim at dangerous shells
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell)) {
            setDirectionToTarget(shell);
            return;
        }
    }

    // If no dangerous shells, aim at opponent tanks
    for (const auto& opponent : opponentTanks) {
        if (isInLineWithTarget(opponent)) {
            setDirectionToTarget(opponent);
            return;
        }
    }
}

bool ComputerPlayer::isInLineWithTarget(const GameObject* target) const {
    return target->getX() == getX() || target->getY() == getY();
}

void ComputerPlayer::setDirectionToTarget(const GameObject* target) {
    if (target->getX() == getX()) {
        setDirection(target->getY() < getY() ? Direction::UP : Direction::DOWN);
    }
    else {
        setDirection(target->getX() < getX() ? Direction::LEFT : Direction::RIGHT);
    }
}

bool ComputerPlayer::isShellAimedAtMe(const Shell* shell) const {
    if (!shell || !isInLineWithTarget(shell)) return false;

    return (shell->getDirection() == Direction::UP && shell->getY() > getY()) ||
        (shell->getDirection() == Direction::DOWN && shell->getY() < getY()) ||
        (shell->getDirection() == Direction::LEFT && shell->getX() > getX()) ||
        (shell->getDirection() == Direction::RIGHT && shell->getX() < getX());
}

bool ComputerPlayer::isMyTanksInDirection(const std::vector<Tank*>& myTanks, Direction shootDir) const {
    for (const auto& tank : myTanks) {
        if (tank == this) continue;
        if (!isInLineWithTarget(tank)) continue;

        return (shootDir == Direction::UP && tank->getY() < getY()) ||
            (shootDir == Direction::DOWN && tank->getY() > getY()) ||
            (shootDir == Direction::LEFT && tank->getX() < getX()) ||
            (shootDir == Direction::RIGHT && tank->getX() > getX());
    }
    return false;
}