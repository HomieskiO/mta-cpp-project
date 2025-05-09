#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "GameManager.h"
#include "Screen.h"

constexpr auto START_GAME = '1';
constexpr auto CHANGE_GAME_COLOR = '7';
constexpr auto CHANGE_GAME_SCREEN = '6';
constexpr auto INSTRUCTIONS = '8';
constexpr auto EXIT = '9';

class MenuManager {
public:
    MenuManager();          
    void openMenu();

private:
    GameManager* gameManager;
    bool coloredGame;
	std::string selectedScreenFile;
	std::vector<Screen> screens;

    void showInstructionsAndKeys();
    void selectScreen();
    std::vector<std::string> getAvailableScreens();
    void displayScreens(const std::vector<std::string>& screens);
	void displayChangeScreenMenu();

};
