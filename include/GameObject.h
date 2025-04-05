#pragma once

class GameObject {
protected:
    char symbol;
    int x;
    int y;
    bool alive;

public:
    GameObject(char symbol, int x, int y);

    char getSymbol() const;
    void setSymbol(char symbol);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    bool isAlive() const;
    void setState(bool state);

    virtual void draw() const;
    bool collidesWith(const GameObject& object) const;

    virtual ~GameObject();
};
