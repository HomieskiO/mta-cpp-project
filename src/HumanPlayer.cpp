#include "HumanPlayer.h"
#include "ioUtils.h"
#include "Shell.h"

HumanPlayer::HumanPlayer(int x, int y, const PlayerControls& controls, int color)
    : Tank(x, y, controls, color) {}

bool HumanPlayer::shouldShoot(const std::vector<Tank*>& opponentTanks) {
    return isKeyPressed(getControls().shoot);
}

void HumanPlayer::makeMove(const std::vector<Shell*>& shells,
                          const std::vector<Tank*>& tanks,
                          const std::vector<Wall>& walls) {
    if (shouldShoot)) {
        return;
    }
    if (isKeyPressed(getControls().stay)) {
        setMovementState(MovementState::STAY);
        setRotation(0);
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
    }
    if (isKeyPressed(getControls().rightTrackBackward) && isKeyPressed(getControls().leftTrackBackward)) {
        setMovementState(MovementState::BACKWARD);
    }
    if (isKeyPressed(getControls().rightTrackForward) && !isKeyPressed(getControls().leftTrackForward) && !isKeyPressed(getControls().leftTrackBackward)) {
        setRotation(-45);
    }
    if (isKeyPressed(getControls().rightTrackBackward) && !isKeyPressed(getControls().leftTrackForward) && !isKeyPressed(getControls().leftTrackBackward)) {
        setRotation(45);
    }
    if (isKeyPressed(getControls().leftTrackForward) && !isKeyPressed(getControls().rightTrackForward) && !isKeyPressed(getControls().rightTrackBackward)) {
        setRotation(45);
    }
    if (isKeyPressed(getControls().leftTrackBackward) && !isKeyPressed(getControls().rightTrackForward) && !isKeyPressed(getControls().rightTrackBackward)) {
        setRotation(-45);
    }
    if (isKeyPressed(getControls().leftTrackForward) && isKeyPressed(getControls().rightTrackBackward)) {
        setRotation(90);
    }
    if (isKeyPressed(getControls().leftTrackBackward) && isKeyPressed(getControls().rightTrackForward)) {
        setRotation(-90);
    }
} 