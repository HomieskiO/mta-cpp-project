#include "Wall.h"
#include "IOUtils.h"
# include <iostream>

Wall::Wall(int x, int y): GameObject(WALL_SYMBOL, x, y){}
Wall::Wall(int x, int y, int color) : GameObject(WALL_SYMBOL, x, y, color) {}

void Wall::hit() {
    if (hits > 0)
        hits--;

    if (hits == 0) {
        setState(false);
        gotoxy(x, y);
        std::cout << " ";
    }
}