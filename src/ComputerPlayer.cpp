#include "ComputerPlayer.h"
#include "GameManager.h"
#include <algorithm>
#include <cmath>

// General comment : computer player class  only aims and rotates, all shooting and movement is actually done in GameManager

ComputerPlayer::ComputerPlayer(int x, int y, int color, int playerId, int tankId)
    : Tank(x, y, P2_CONTROLS, color) {  // Using P2_CONTROLS as default value
    this->playerId = playerId;
    this->tankId = tankId;
}

bool ComputerPlayer::shouldShoot(const std::vector<Tank*>& opponentTanks) {
    // Only shoot if we can shoot and have a valid target
    return canShoot() && canShootTarget(opponentTanks, currentShells);
}

void ComputerPlayer::makeMove(const std::vector<Shell*>& shells,
    const std::vector<Tank*>& opponentTanks,
    const std::vector<Wall>& walls,
    GameRecorder& gameRecorder) {
    // Store current shells for shouldShoot
    currentShells = shells;

    // First priority: move away from shells
    if (isShellChasing(shells)) {
        for (const auto& shell : shells) {
            if (std::abs(shell->getX() - getX()) <= DANGER_DISTANCE &&
                std::abs(shell->getY() - getY()) <= DANGER_DISTANCE) {
                moveAwayFromShell(shell, gameRecorder);
                return;
            }
        }
    }
    // Second priority: Shoot targets if possible
    if (canShootTarget(opponentTanks, shells)) {
        aimAtTarget(opponentTanks, shells, gameRecorder);
        return;

        // TODO: Third priority will be moving towards opponent
    }
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

void ComputerPlayer::moveAwayFromShell(const Shell* shell, GameRecorder& gameRecorder) {
    int dx = getX() - shell->getX();
    int dy = getY() - shell->getY();

    // Move in the opposite direction of the shell
    ActionType action;
    if (std::abs(dx) > std::abs(dy)) {
        setDirection(dx > 0 ? Direction::RIGHT : Direction::LEFT);
        if (dx > 0) {
            action = ActionType::DIRECTION_RIGHT;
        }
        else {
            action = ActionType::DIRECTION_LEFT;
        }
    }
    else {
        setDirection(dy > 0 ? Direction::DOWN : Direction::UP);
        if (dy > 0) {
            action = ActionType::DIRECTION_DOWN;
        }
        else {
            action = ActionType::DIRECTION_UP;
        }
    }
    gameRecorder.logAction(playerId, tankId, action);
    // TODO: Set movement state as backward or stay as well and not only forward
    // TODO: movement of tanks should avoid mines
    setMovementState(MovementState::FORWARD);
    gameRecorder.logAction(playerId, tankId, ActionType::MOVE_FORWARD);
    return;
}

bool ComputerPlayer::canShootTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells) const {
    // First check for shells aimed at us
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell) && (shell->getX() == getX() || shell->getY() == getY())) {
            return true;
        }
    }

    // Try to shoot at opponent tanks
    for (const auto& opponent : opponentTanks) {
        if (opponent->getX() == getX() || opponent->getY() == getY()) {
            return true;
        }
    }
    return false;
}

void ComputerPlayer::aimAtTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells, GameRecorder& gameRecorder) {
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell)) {
            if (shell->getX() == getX()) {
                setDirection(shell->getY() < getY() ? Direction::UP : Direction::DOWN);
                if (shell->getY() < getY()) {
                    gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_UP);
                }
                else {
                    gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_DOWN);
                }
                return;
            }
            if (shell->getY() == getY()) {
                setDirection(shell->getX() < getX() ? Direction::LEFT : Direction::RIGHT);
                if (shell->getX() < getX()) {
                    gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_LEFT);
                }
                else {
                    gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_RIGHT);
                }
                return;
            }
        }
    }

    // If no dangerous shells, aim at opponent tanks
    for (const auto& opponent : opponentTanks) {
        if (opponent->getX() == getX()) {
            setDirection(opponent->getY() < getY() ? Direction::UP : Direction::DOWN);
            if (opponent->getY() < getY()) {
                gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_UP);
            }
            else {
                gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_DOWN);
            }
            return;
        }
        if (opponent->getY() == getY()) {
            setDirection(opponent->getX() < getX() ? Direction::LEFT : Direction::RIGHT);
            if (opponent->getX() < getX()) {
                gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_LEFT);
            }
            else {
                gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_RIGHT);
            }
            return;
        }
    }
}

// TODO: change those to avoid code duplications
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