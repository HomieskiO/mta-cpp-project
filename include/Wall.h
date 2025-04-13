#pragma once
#include "GameObject.h"

constexpr auto WALL_SYMBOL = '#';

class Wall : public GameObject {
private:
    int hits = 2;

public:
    Wall(int x, int y);
    Wall(int x, int y, int color);
    void hit();
};


