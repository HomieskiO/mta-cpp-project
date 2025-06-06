#pragma once
#include "GameObject.h"
#include "MovingObject.h"
#include "Cannon.h"
#include "Shell.h"
#include "Wall.h"
#include <string>

constexpr auto TANK_SYMBOL = 'O';
constexpr auto SHOOT_COOLDOWN = 5;

struct PlayerControls {
    int leftTrackForward;
    int leftTrackBackward;
    int rightTrackForward;
    int rightTrackBackward;
    int stay;
    int shoot;
    int switchActiveTank;
};

constexpr PlayerControls P1_CONTROLS = {
    'Q', 'A', 'E', 'D', 'S', 'W', 'Z'
};

constexpr PlayerControls P2_CONTROLS = {
    'U', 'J', 'O', 'L', 'K', 'I', 'M'
};


class Tank : public MovingObject {
private:
	PlayerControls controls;
    Cannon* cannon;
    Shell* shell;
    int cooldown;
    int rotation;
    int lastRotation;
protected:
    virtual bool canRotateCannon() const;
    virtual void handleCollision(const GameObject& other);
public:
	Tank(int x, int y, PlayerControls controls);
    Tank(int x, int y, PlayerControls controls, int color);
    PlayerControls getControls() const;
	void setControls(PlayerControls controls);
    void setRotation(int rotation);
    int getRotation() const;
    int getLastRotation() const;
    virtual void rotateCannon();
    virtual void rotateCannon(int angle);
    virtual void move() override;
    virtual bool canMove() const override;
    virtual void updatePosition() override;
    virtual bool canShoot();
	void draw() const override;
	int getCannonX() const;
	int getCannonY() const;
    int getCooldown();
    void setCooldown(int cooldown);
    Cannon* getCannon();
    void removeCannon();
    int getColor();
    void alignCannon();
    Tank(const Tank&) = default;
    Tank& operator=(const Tank&) = default;
    virtual ~Tank();
    virtual void makeMove(const std::vector<Shell*>& shells,
                         const std::vector<Tank*>& tanks,
                         const std::vector<Wall>& walls) = 0;
};
