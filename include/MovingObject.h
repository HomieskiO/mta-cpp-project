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
    int prevX;
    int prevY;
	MovementState movementState;
	Direction direction;
public:
	MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState);
    MovingObject(char symbol, int x, int y, Direction direction, MovementState movementState, int color);
    int getPrevX() const;
    int getPrevY() const;
	MovementState getMovementState() const;	
	void setMovementState(MovementState state);
    Direction getDirection() const;
    void setDirection(Direction direction);
    void draw() const override;
	virtual void move(); 
    virtual bool canMove() const; 
    virtual void updatePosition(); 
    virtual ~MovingObject() = default; 
};