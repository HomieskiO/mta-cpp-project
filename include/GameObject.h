#pragma once

constexpr auto WHITE_COLOR = 7;


class GameObject {
protected:
    char symbol;
    int x;
    int y;
    bool alive;
    int color;

public:
    GameObject(char symbol, int x, int y);
    GameObject(char symbol, int x, int y, int color);

    char getSymbol() const;
    void setSymbol(char symbol);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    int getColor() const;
    void setColor(int color);

    bool isAlive() const;
    void setState(bool state);

    virtual void draw() const;
    bool collidesWith(const GameObject& object) const;
    bool collidesWith(const GameObject* object) const;

    virtual ~GameObject();
};
