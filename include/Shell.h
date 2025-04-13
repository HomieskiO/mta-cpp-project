#pragma once
#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include <string>

constexpr auto SHELL_SYMBOL = '*';

class Shell : public MovingObject {
public:
    Shell(int x, int y, Direction tankDirection);
    Shell(int x, int y, Direction tankDirection, int color);
};