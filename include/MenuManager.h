#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "GameManager.h"

constexpr auto START_GAME = '1';
constexpr auto CHANGE_GAME_COLOR = '7';
constexpr auto INSTRUCTIONS = '8';
constexpr auto EXIT = '9';
constexpr auto SELECT_SCREEN = '2';

class MenuManager {
public:
    MenuManager();          
    void openMenu();

private:
    GameManager* gameManager;
    bool coloredGame;
    int selectedScreen;

    void showInstructionsAndKeys();
    void selectScreen();
    std::vector<std::string> getAvailableScreens();
    void displayScreens(const std::vector<std::string>& screens);
};
