#pragma once
#include "GameObject.h"

constexpr auto MINE_SYMBOL = '@';

class Mine : public GameObject {
public:
    Mine(int x, int y);
    Mine(int x, int y, int color);

    Mine(const Mine&) = default;
    Mine& operator=(const Mine&) = default;
    ~Mine() = default;
};