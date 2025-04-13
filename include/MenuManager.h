#pragma once
#include <iostream>
#include "GameManager.h"

constexpr auto START_GAME = '1';
constexpr auto CHANGE_GAME_COLOR = '7';
constexpr auto INSTRUCTIONS = '8';
constexpr auto EXIT = '9';

class MenuManager {
public:
    MenuManager();          
    void openMenu();

private:
    GameManager* gameManager;
    bool coloredGame;

    void showInstructionsAndKeys();
};
