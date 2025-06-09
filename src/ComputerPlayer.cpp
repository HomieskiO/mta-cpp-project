#include "ComputerPlayer.h"
#include <cmath>
#include <algorithm>
#include <iostream>

ComputerPlayer::ComputerPlayer(int x, int y, int color)
    : Tank(x, y, P2_CONTROLS, color) {
}

ComputerPlayer::PointDistance ComputerPlayer::calculateDistance(int x1, int y1, int x2, int y2) const {
    PointDistance dist;
    dist.xDistance = std::abs(x1 - x2);
    dist.yDistance = std::abs(y1 - y2);
    dist.distance = dist.xDistance + dist.yDistance;
    return dist;
}

void ComputerPlayer::makeMove(const std::vector<Shell*>& shells,
    const std::vector<Tank*>& opponentTanks,
    const std::vector<Wall>& walls) {
    if (shouldMoveTank(shells)) {
        moveTankFromShell(shells);
        return;
    }
    if (shouldShoot(opponentTanks)) {
        aimAtTank(opponentTanks); // should also aim at possible shells
        return;
    }


    Tank* nearestOpponent = nullptr;
    int minDistance = INT_MAX;
    for (const auto& opponent : opponentTanks) {
        PointDistance dist = calculateDistance(getX(), getY(), opponent->getX(), opponent->getY());
        if (dist.distance < minDistance) {
            minDistance = dist.distance;
            nearestOpponent = opponent;
        }
    }

    if (nearestOpponent && minDistance > SHOOTING_RANGE) {
        // Move towards opponent
        PointDistance dist = calculateDistance(getX(), getY(), nearestOpponent->getX(), nearestOpponent->getY());
        if (dist.xDistance > dist.yDistance) {
            setDirection(dist.xDistance > 0 ? Direction::RIGHT : Direction::LEFT);
        }
        else {
            setDirection(dist.yDistance > 0 ? Direction::DOWN : Direction::UP);
        }
        Tank::move();
    }
}

bool ComputerPlayer::shouldMoveTank(const std::vector<Shell*>& shells) {
    return isCloseToShell(shells) != nullptr;
}

bool ComputerPlayer::shouldShoot(const std::vector<Tank*>& opponentTanks) {
    for (const auto& opponent : opponentTanks) {
        PointDistance dist = calculateDistance(getX(), getY(), opponent->getX(), opponent->getY());
        if (dist.distance <= SHOOTING_RANGE && canShoot()) {
            return true;
        }
    }
    return false;
}

Shell* ComputerPlayer::isCloseToShell(const std::vector<Shell*>& shells) {
    for (const auto& shell : shells) {
        PointDistance dist = calculateDistance(getX(), getY(), shell->getX(), shell->getY());

        if (dist.xDistance <= DANGER_DISTANCE && dist.yDistance <= DANGER_DISTANCE) {
            Direction shellDir = shell->getDirection();
            bool isInDirection = false;

            switch (shellDir) {
            case Direction::UP:
                isInDirection = shell->getY() > getY();
                break;
            case Direction::DOWN:
                isInDirection = shell->getY() < getY();
                break;
            case Direction::LEFT:
                isInDirection = shell->getX() > getX();
                break;
            case Direction::RIGHT:
                isInDirection = shell->getX() < getX();
                break;
            default:
                break;
            }

            if (isInDirection) return shell;
        }
    }
    return nullptr;
}

bool ComputerPlayer::isOpponentInRange(const std::vector<Tank*>& opponentTanks) {
    for (const auto& opponent : opponentTanks) {
        PointDistance dist = calculateDistance(getX(), getY(), opponent->getX(), opponent->getY());
        if (dist.distance <= SHOOTING_RANGE) {
            return true;
        }
    }
    return false;
}

void ComputerPlayer::moveTankFromShell(const std::vector<Shell*>& shells) {
    Shell* dangerousShell = isCloseToShell(shells);
    if (!dangerousShell) {
        return;
    }

    Direction shellDir = dangerousShell->getDirection();
    Direction moveDir;

    switch (shellDir) {
    case Direction::UP:
        moveDir = Direction::DOWN;
        break;
    case Direction::DOWN:
        moveDir = Direction::UP;
        break;
    case Direction::LEFT:
        moveDir = Direction::RIGHT;
        break;
    case Direction::RIGHT:
        moveDir = Direction::LEFT;
        break;
    default:
        return;
    }

    this->setDirection(moveDir);
    this->move();
}

void ComputerPlayer::aimAtTank(const std::vector<Tank*>& opponentTanks) {
    Tank* nearestOpponent = nullptr;
    int minDistance = INT_MAX;

    for (const auto& opponent : opponentTanks) {
        PointDistance dist = calculateDistance(getX(), getY(), opponent->getX(), opponent->getY());
        if (dist.distance < minDistance) {
            minDistance = dist.distance;
            nearestOpponent = opponent;
        }
    }

    if (nearestOpponent) {
        int dx = nearestOpponent->getX() - getX();
        int dy = nearestOpponent->getY() - getY();

		int angle = //TODO: use same angle calculation as in HumanPlayer
            (dx == 0 && dy < 0) ? 0 :
            (dx > 0 && dy < 0) ? 45 :
            (dx > 0 && dy == 0) ? 90 :
            (dx > 0 && dy > 0) ? 135 :
            (dx == 0 && dy > 0) ? 180 :
            (dx < 0 && dy > 0) ? 225 :
            (dx < 0 && dy == 0) ? 270 :
            (dx < 0 && dy < 0) ? 315 :
            0; // default

        rotateCannon(angle);
    }
}


bool ComputerPlayer::isPathClear(const Tank* target, const std::vector<Wall>& walls) {
    for (const auto& wall : walls) {
        int wx = wall.getX();
        int wy = wall.getY();

        if (wx >= std::min(getX(), target->getX()) && wx <= std::max(getX(), target->getX()) &&
            wy >= std::min(getY(), target->getY()) && wy <= std::max(getY(), target->getY())) {
            return false;
        }
    }

    return true;
}