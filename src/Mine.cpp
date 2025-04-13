#include "mine.h"

Mine::Mine(int x, int y): GameObject(MINE_SYMBOL, x, y){}
Mine::Mine(int x, int y, int color) : GameObject(MINE_SYMBOL, x, y, color) {}