#pragma once

enum class Direction {
    UP,
    UPRIGHT,
    UPLEFT,
    LEFT,
    DOWN,
    DOWNRIGHT,
    DOWNLEFT,
    RIGHT
};

class GameObject {
protected:
    char symbol;
    int x;
    int y;
    Direction direction;
    bool alive;

public:
    GameObject(char symbol, int x, int y, Direction direction);

    char getSymbol() const;
    void setSymbol(char symbol);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    Direction getDirection() const;
    void setDirection(Direction direction);

    bool isAlive() const;
    void setState(bool state);

    void draw() const;
    bool collidesWith(const GameObject& object) const;

    virtual ~GameObject();
};
