#pragma once
#include <iostream>
#include "GameManager.h"
#define START_GAME '1'
#define INSTRUCTIONS '8'
#define EXIT '9'

class MenuManager {
public:
    MenuManager();          

    void openMenu();
    void startGame();

private:
    GameManager gameManager;

    void showInstructionsAndKeys();
};
