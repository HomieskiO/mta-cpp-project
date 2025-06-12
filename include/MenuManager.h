#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "GameManager.h"
#include "Screen.h"
#include "ComputerPlayer.h"

constexpr auto START_GAME = '1';
constexpr auto CHANGE_GAME_COLOR = '7';
constexpr auto CHANGE_GAME_SCREEN = '6';
constexpr auto INSTRUCTIONS = '8';
constexpr auto EXIT = '9';

constexpr auto HUMAN_VS_HUMAN = '1';
constexpr auto HUMAN_VS_COMPUTER = '2';
constexpr auto COMPUTER_VS_COMPUTER = '3';

class MenuManager {
public:
    MenuManager();          
    void openMenu();
    MenuManager(const MenuManager&) = delete;
    MenuManager& operator=(const MenuManager&) = delete;
    ~MenuManager() = default;

private:
    GameManager* gameManager;
    bool coloredGame;
    std::string selectedScreenFile;
    std::vector<Screen> screens;
    PlayerType player1Type;
    PlayerType player2Type;

    void showInstructionsAndKeys();
    void selectScreen();
    void selectGameMode();
    void displayGameModeMenu();
    std::vector<std::string> getAvailableScreens();
    void displayScreens(const std::vector<std::string>& screens);
    void displayChangeScreenMenu();
};
