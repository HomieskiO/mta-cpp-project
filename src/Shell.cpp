#include "MovingObject.h"
#include "Shell.h"
#include "GameManager.h"

Shell::Shell(int x, int y, Direction tankDirection) : MovingObject(SHELL_SYMBOL, x, y, tankDirection, MovementState::FORWARD) {
}

Shell::Shell(int x, int y, Direction tankDirection, int color) : MovingObject(SHELL_SYMBOL, x, y, tankDirection, MovementState::FORWARD, color) {
}
