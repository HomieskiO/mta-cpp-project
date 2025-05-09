#include "GameObject.h"
#include "ioUtils.h"

GameObject::GameObject(char symbol, int x, int y) {
    this->symbol = symbol;
    this->x = x;
    this->y = y;
    this->alive = true;
    this->color = WHITE_COLOR;
}

GameObject::GameObject(char symbol, int x, int y, int color) {
    this->symbol = symbol;
    this->x = x;
    this->y = y;
    this->alive = true;
    this->color = color;
}

char GameObject::getSymbol() const {
    return symbol;
}

void GameObject::setSymbol(char symbol) {
    this->symbol = symbol;
}

int GameObject::getColor() const {
    return color;
}

void GameObject::setColor(int color) {
    this->color = color;
}

int GameObject::getX() const {
    return x;
}

void GameObject::setX(int x) {
    this->x = x;
}

int GameObject::getY() const {
    return y;
}

void GameObject::setY(int y) {
    this->y = y;
}

bool GameObject::isAlive() const { 
    return alive;
}

void GameObject::setState(bool state) {
    this->alive = state;
}

void GameObject::draw() const { 
    gotoxy(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    std::cout << symbol;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_COLOR);
}

bool GameObject::collidesWith(const GameObject& object) const { 
    return this->x == object.getX() && this->y == object.getY();
}

bool GameObject::collidesWith(const GameObject* object) const {
    return this->x == object->getX() && this->y == object->getY();
}

GameObject::~GameObject() {
}
