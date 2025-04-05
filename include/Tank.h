#pragma once
#include "GameObject.h"

constexpr auto TANK_SYMBOL = 'O';

class Tank : public GameObject {
public:
	Tank(int x, int y);
};
