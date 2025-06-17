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
                moveAwayFromShell(shell, gameRecorder);
                return;
            }
        }
    }
    else {
        setMovementState(MovementState::STAY);
        gameRecorder.logAction(playerId, tankId, ActionType::STAY);
    }
    // Second priority: Shoot targets if possible
    if (canShootTarget(opponentTanks, shells)) {
        aimAtTarget(opponentTanks, shells, gameRecorder);
        return;
    }
	//Third priority: Making it moving forward to prevent game from freezing if no shells or targets are present
    setMovementState(MovementState::FORWARD);
	gameRecorder.logAction(playerId, tankId, ActionType::MOVE_FORWARD);
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


    bool moveHorizontally = std::abs(dx) > std::abs(dy);
    Direction newDirection = moveHorizontally ?
        (dx > 0 ? Direction::RIGHT : Direction::LEFT) :
        (dy > 0 ? Direction::DOWN : Direction::UP);
    setDirection(newDirection);
    ActionType action;
    switch (newDirection) {
        case Direction::LEFT:
            action = ActionType::DIRECTION_LEFT;
            break;
        case Direction::RIGHT:
            action = ActionType::DIRECTION_RIGHT;
            break;
        case Direction::UP:
            action = ActionType::DIRECTION_UP;
            break;
        case Direction::DOWN:
            action = ActionType::DIRECTION_DOWN;
            break;
        default:
            action = ActionType::STOP_ROTATE;
    }
    gameRecorder.logAction(playerId, tankId, action);

    bool shellMovingTowards = moveHorizontally ?
        ((dx > 0 && shell->getDirection() == Direction::LEFT) ||
            (dx < 0 && shell->getDirection() == Direction::RIGHT)) :
        ((dy > 0 && shell->getDirection() == Direction::UP) ||
            (dy < 0 && shell->getDirection() == Direction::DOWN));

    setMovementState(shellMovingTowards ? MovementState::BACKWARD : MovementState::FORWARD);
    if (shellMovingTowards) {
        gameRecorder.logAction(playerId, tankId, ActionType::MOVE_BACKWARD);
    }
    else {
        gameRecorder.logAction(playerId, tankId, ActionType::MOVE_FORWARD);
    }
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

void ComputerPlayer::aimAtTarget(const std::vector<Tank*>& opponentTanks, const std::vector<Shell*>& shells, GameRecorder& gameRecorder) {
    // First try to aim at dangerous shells
    for (const auto& shell : shells) {
        if (isShellAimedAtMe(shell)) {
            setDirectionToTarget(shell, gameRecorder);
            return;
        }
    }

    // If no dangerous shells, aim at opponent tanks
    for (const auto& opponent : opponentTanks) {
        if (isInLineWithTarget(opponent)) {
            setDirectionToTarget(opponent, gameRecorder);
            return;
        }
    }
}

bool ComputerPlayer::isInLineWithTarget(const GameObject* target) const {
    return target->getX() == getX() || target->getY() == getY();
}

void ComputerPlayer::setDirectionToTarget(const GameObject* target, GameRecorder& gameRecorder) {
    if (target->getX() == getX()) {
        setDirection(target->getY() < getY() ? Direction::UP : Direction::DOWN);
        if (target->getY() < getY()) {
            gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_UP);
        }
        else {
            gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_DOWN);
        }
    }
    else {
        setDirection(target->getX() < getX() ? Direction::LEFT : Direction::RIGHT);
        if (target->getX() < getX()) {
            gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_LEFT);
        }
        else {
            gameRecorder.logAction(playerId, tankId, ActionType::DIRECTION_RIGHT);
        }
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