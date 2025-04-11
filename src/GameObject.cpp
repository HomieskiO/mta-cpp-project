#include "GameObject.h"
#include "IOUtils.h"

GameObject::GameObject(char symbol, int x, int y) {
    this->symbol = symbol;
    this->x = x;
    this->y = y;
    this->alive = true;
}

char GameObject::getSymbol() const {
    return symbol;
}

void GameObject::setSymbol(char symbol) {
    this->symbol = symbol;
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
    std::cout << symbol;
}

bool GameObject::collidesWith(const GameObject& object) const { 
    return this->x == object.getX() && this->y == object.getY();
}

bool GameObject::collidesWith(const GameObject* object) const {
    return this->x == object->getX() && this->y == object->getY();
}

GameObject::~GameObject() {
}
