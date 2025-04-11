#pragma once
#include "GameObject.h"

enum class MovementState {
    FORWARD,
	BACKWARD,
	STAY
};

enum class Direction {
    UP,
    UPRIGHT,
    RIGHT,
    DOWNRIGHT,
    DOWN,
    DOWNLEFT,
    LEFT,
    UPLEFT
};

class MovingObject : public GameObject {
protected: 
	MovementState movementState;
	Direction direction;
public:
	MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState);
    MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState, int color);
	MovementState getMovementState() const;	
	void setMovementState(MovementState state);
    Direction getDirection() const;
    void setDirection(Direction direction);
	virtual void move() = 0;
};

