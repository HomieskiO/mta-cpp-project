#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "GameManager.h"
#include "Screen.h"

constexpr auto START_GAME = '1';
constexpr auto CHANGE_GAME_SCREEN = '6';
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

    std::vector<Screen> screens;
    std::string selectedScreenFile;

    void displayChangeScreenMenu();
    void showInstructionsAndKeys();
};
