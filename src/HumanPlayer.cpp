#include "HumanPlayer.h"
#include "ioUtils.h"
#include "Shell.h"

HumanPlayer::HumanPlayer(int x, int y, const PlayerControls& controls, int color)
    : Tank(x, y, controls, color) {}

HumanPlayer::HumanPlayer(int x, int y, const PlayerControls& controls, int color, int playerId, int tankId)
    : Tank(x, y, controls, color) {
    this->playerId = playerId;
    this->tankId = tankId;
}

bool HumanPlayer::shouldShoot(const std::vector<Tank*>& opponentTanks) {
    return isKeyPressed(getControls().shoot);
}

void HumanPlayer::makeMove(const std::vector<Shell*>& shells,
                          const std::vector<Tank*>& tanks,
                          const std::vector<Wall>& walls,
                          GameRecorder& gameRecorder) {
    if (shouldShoot(tanks)) {
        return;
    }
    if (isKeyPressed(getControls().stay)) {
        setMovementState(MovementState::STAY);
        gameRecorder.logAction(playerId, tankId, ActionType::STAY);
        setRotation(0);
        gameRecorder.logAction(playerId, tankId, ActionType::STOP_ROTATE);
        return;
    }
    if (isKeyPressed(getControls().rightTrackForward) && isKeyPressed(getControls().rightTrackBackward)) {
        return;
    }
    if (isKeyPressed(getControls().leftTrackForward) && isKeyPressed(getControls().leftTrackBackward)) {
        return;
    }
    if (isKeyPressed(getControls().rightTrackForward) && isKeyPressed(getControls().leftTrackForward)) {
        setMovementState(MovementState::FORWARD);
        gameRecorder.logAction(playerId, tankId, ActionType::MOVE_FORWARD);
    }
    if (isKeyPressed(getControls().rightTrackBackward) && isKeyPressed(getControls().leftTrackBackward)) {
        setMovementState(MovementState::BACKWARD);
        gameRecorder.logAction(playerId, tankId, ActionType::MOVE_BACKWARD);
    }
    if (isKeyPressed(getControls().rightTrackForward) && !isKeyPressed(getControls().leftTrackForward) && !isKeyPressed(getControls().leftTrackBackward)) {
        setRotation(-45);
        gameRecorder.logAction(playerId, tankId, ActionType::ROTATE_LEFT_45);
    }
    if (isKeyPressed(getControls().rightTrackBackward) && !isKeyPressed(getControls().leftTrackForward) && !isKeyPressed(getControls().leftTrackBackward)) {
        setRotation(45);
        gameRecorder.logAction(playerId, tankId, ActionType::ROTATE_RIGHT_45);
    }
    if (isKeyPressed(getControls().leftTrackForward) && !isKeyPressed(getControls().rightTrackForward) && !isKeyPressed(getControls().rightTrackBackward)) {
        setRotation(45);
        gameRecorder.logAction(playerId, tankId, ActionType::ROTATE_RIGHT_45);
    }
    if (isKeyPressed(getControls().leftTrackBackward) && !isKeyPressed(getControls().rightTrackForward) && !isKeyPressed(getControls().rightTrackBackward)) {
        setRotation(-45);
        gameRecorder.logAction(playerId, tankId, ActionType::ROTATE_LEFT_45);
    }
    if (isKeyPressed(getControls().leftTrackForward) && isKeyPressed(getControls().rightTrackBackward)) {
        setRotation(90);
        gameRecorder.logAction(playerId, tankId, ActionType::ROTATE_RIGHT_90);
    }
    if (isKeyPressed(getControls().leftTrackBackward) && isKeyPressed(getControls().rightTrackForward)) {
        setRotation(-90);
        gameRecorder.logAction(playerId, tankId, ActionType::ROTATE_LEFT_90);
    }
} 