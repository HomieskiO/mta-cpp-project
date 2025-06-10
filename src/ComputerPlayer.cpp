#include "ComputerPlayer.h"
#include "GameManager.h"
#include <algorithm>
#include <cmath>

ComputerPlayer::ComputerPlayer(int x, int y, int color)
    : Tank(x, y, P2_CONTROLS, color) {  // Using P2_CONTROLS as default, but computer doesn't use them
}

bool ComputerPlayer::shouldShoot(const std::vector<Tank*>& opponentTanks) {
    // Only shoot if we can shoot and have a valid target
    return canShoot() && canShootTarget(opponentTanks, currentShells);
}

void ComputerPlayer::makeMove(const std::vector<Shell*>& shells,
    const std::vector<Tank*>& opponentTanks,
    const std::vector<Wall>& walls) {
    // Store current shells for shouldShoot
    currentShells = shells;

    // First priority: Avoid shells
    if (isShellChasing(shells)) {
        for (const auto& shell : shells) {
            if (std::abs(shell->getX() - getX()) <= DANGER_DISTANCE &&
                std::abs(shell->getY() - getY()) <= DANGER_DISTANCE) {
                moveAwayFromShell(shell);
                return;
            }
        }
    }

    // Second priority: Shoot targets if possible
    if (canShootTarget(opponentTanks, shells)) {
        aimAtTarget(opponentTanks, shells);
        return;
    }

    // Third priority: Move towards opponent
    moveTowardsOpponent(opponentTanks);
}

Tank* ComputerPlayer::getClosestOpponent(const std::vector<Tank*>& opponentTanks) const {
    Tank* closest = nullptr;
    int minDist = INT_MAX;

    for (const auto& opponent : opponentTanks) {
        int dx = opponent->getX() - getX();
        int dy = opponent->getY() - getY();
        int dist = dx * dx + dy * dy;  // Use squared distance for efficiency
        if (dist < minDist) {
            minDist = dist;
            closest = opponent;
        }
    }
    return closest;
}

void ComputerPlayer::moveTowardsOpponent(const std::vector<Tank*>& opponentTanks) {
    Tank* closest = getClosestOpponent(opponentTanks);
    if (!closest) return;

    int dx = closest->getX() - getX();
    int dy = closest->getY() - getY();

    // Move in the direction of the largest difference
    if (std::abs(dx) > std::abs(dy)) {
        setDirection(dx > 0 ? Direction::RIGHT : Direction::LEFT);
    }
    else {
        setDirection(dy > 0 ? Direction::DOWN : Direction::UP);
    }
    Tank::move();
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

    // Move in the opposite direction of the shell
    if (std::abs(dx) > std::abs(dy)) {
        setDirection(dx > 0 ? Direction::RIGHT : Direction::LEFT);
    }
    else {
        setDirection(dy > 0 ? Direction::DOWN : Direction::UP);
    }
    Tank::move();
}

bool ComputerPlayer::canShootTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells) const {
    // First check for shells aimed at us
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell) && (shell->getX() == getX() || shell->getY() == getY())) {
            return true;
        }
    }

    // Then check for opponent tanks
    for (const auto& opponent : opponentTanks) {
        // Check if opponent is in line of sight
        if (opponent->getX() == getX() || opponent->getY() == getY()) {
            return true;
        }
    }
    return false;
}

void ComputerPlayer::aimAtTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells) {
    // First try to shoot shells aimed at us
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell)) {
            if (shell->getX() == getX()) {
                setDirection(shell->getY() < getY() ? Direction::UP : Direction::DOWN);
                return;
            }
            if (shell->getY() == getY()) {
                setDirection(shell->getX() < getX() ? Direction::LEFT : Direction::RIGHT);
                return;
            }
        }
    }

    // If no dangerous shells, aim at opponent tanks
    for (const auto& opponent : opponentTanks) {
        if (opponent->getX() == getX()) {
            setDirection(opponent->getY() < getY() ? Direction::UP : Direction::DOWN);
            return;
        }
        if (opponent->getY() == getY()) {
            setDirection(opponent->getX() < getX() ? Direction::LEFT : Direction::RIGHT);
            return;
        }
    }
}

bool ComputerPlayer::isShellAimedAtMe(const Shell* shell) const {
    if (!shell) return false;

    // Check if shell is moving towards us
    Direction shellDir = shell->getDirection();
    int dx = getX() - shell->getX();
    int dy = getY() - shell->getY();

    switch (shellDir) {
    case Direction::UP:
        return shell->getY() > getY() && shell->getX() == getX();
    case Direction::DOWN:
        return shell->getY() < getY() && shell->getX() == getX();
    case Direction::LEFT:
        return shell->getX() > getX() && shell->getY() == getY();
    case Direction::RIGHT:
        return shell->getX() < getX() && shell->getY() == getY();
    default:
        return false;
    }
}

bool ComputerPlayer::isMyTanksInDirection(const std::vector<Tank*>& myTanks, Direction shootDir) const {
    for (const auto& tank : myTanks) {
        if (tank == this) continue;
        if (shootDir == Direction::UP && tank->getX() == getX() && tank->getY() < getY()) return true;
        if (shootDir == Direction::DOWN && tank->getX() == getX() && tank->getY() > getY()) return true;
        if (shootDir == Direction::LEFT && tank->getY() == getY() && tank->getX() < getX()) return true;
        if (shootDir == Direction::RIGHT && tank->getY() == getY() && tank->getX() > getX()) return true;
    }
    return false;
}