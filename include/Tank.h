#pragma once
#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include <string>

constexpr auto TANK_SYMBOL = 'O';

struct PlayerControls {
    int leftTrackForward;
    int leftTrackBackward;
    int rightTrackForward;
    int rightTrackBackward;
    int stay;
    int shoot;
};

constexpr PlayerControls P1_CONTROLS = {
    'Q', 'A', 'E', 'D', 'S', 'W'
};

constexpr PlayerControls P2_CONTROLS = {
    'U', 'J', 'O', 'L', 'K', 'I'
};


class Tank : public MovingObject {
private:
	PlayerControls controls;
    Cannon* cannon;
public:
	Tank(int x, int y, PlayerControls controls);
    PlayerControls getControls() const;
	void setControls(PlayerControls controls);
    void rotateCannon(int angle);
    void move() override;
	void draw() const override;
};
