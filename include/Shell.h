#pragma once
#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include <string>

constexpr auto SHELL_SYMBOL = '*';

class Shell : public MovingObject {
private:
    int cannonX;
    int cannonY;

public:
    Shell(int x, int y, int cannonX, int cannonY, Direction cannonDirection);
    void move() override;
};
