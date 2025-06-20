#include <conio.h>
#include <windows.h>
#include "MenuManager.h"
#include "ioUtils.h"
#include "Screen.h"


MenuManager::MenuManager(bool shouldSaveSteps) {
    this->shouldSaveSteps = shouldSaveSteps;
    gameManager = nullptr;
    coloredGame = true;
    selectedScreenFile = "";
    player1Type = PlayerType::HUMAN;
    player2Type = PlayerType::HUMAN;
}

void MenuManager::openMenu() {
    char choice = 0;
    while (choice != START_GAME && choice != INSTRUCTIONS && choice != EXIT) {
        clearScreen();
        std::cout << "=== Main Menu ===\n";
        std::cout << "(1) Start a new Game\n";
        std::cout << "(6) Choose Game Screen\n";
        std::cout << (coloredGame
            ? "(7) Switch to black and white game\n"
            : "(7) Switch to colored game\n");
        std::cout << "(8) Present instructions and keys\n";
        std::cout << "(9) EXIT\n\n";
        std::cout << "Choose an option: ";
        choice = tolower(_getch());

        switch (choice) {
        case START_GAME:
            clearScreen();
            selectGameMode();
            gameManager = new GameManager(coloredGame, selectedScreenFile, player1Type, player2Type, shouldSaveSteps);
            gameManager->startGame();
            delete gameManager;
            choice = 0;
            break;

        case CHANGE_GAME_SCREEN:
            clearScreen();
            displayChangeScreenMenu();
            choice = 0;
            break;

        case CHANGE_GAME_COLOR:
            coloredGame = !coloredGame;
            break;

        case INSTRUCTIONS:
            clearScreen();
            showInstructionsAndKeys();
            choice = 0;
            break;

        case EXIT:
            std::cout << "\nExiting game...\n";
            Sleep(FRAME_RATE_MS);
            return;

        default:
            break;
        }
    }
}

void MenuManager::selectGameMode() {
    char choice;
    bool validChoice = false;

    while (!validChoice) {
        displayGameModeMenu();
        choice = _getch();
        size_t index = choice - '1';

        if (index >= 0 && index < 3) {
            switch (index) {
                case 0:
                    player1Type = PlayerType::HUMAN;
                    player2Type = PlayerType::HUMAN;
                    std::cout << "\nGame mode selected: Human vs Human\n";
                    break;
                case 1:
                    player1Type = PlayerType::HUMAN;
                    player2Type = PlayerType::COMPUTER;
                    std::cout << "\nGame mode selected: Human vs Computer\n";
                    break;
                case 2:
                    player1Type = PlayerType::COMPUTER;
                    player2Type = PlayerType::COMPUTER;
                    std::cout << "\nGame mode selected: Computer vs Computer\n";
                    break;
            }
            validChoice = true;
        }
        else {
            std::cout << "\nInvalid choice. Please try again.\n";
            Sleep(FRAME_RATE_MS);
        }
    }

    std::cout << "\nPress any key to continue...\n";
    _getch();
}

void MenuManager::displayGameModeMenu() {
    clearScreen();
    std::cout << "===================== TANK BATTLE - GAME MODE SETTINGS =====================\n\n";
    std::cout << "1. Human vs Human\n";
    std::cout << "2. Human vs Computer\n";
    std::cout << "3. Computer vs Computer\n\n";
    std::cout << "Choose a game mode by number (1-3): ";
}

void MenuManager::displayChangeScreenMenu() {
    clearScreen();
    std::cout << "===================== TANK BATTLE - SCREEN SETTINGS =====================\n\n";

    if (!Screen::loadAllScreenFiles(screens)) {
        std::cout << "No valid screen files found (must match *.screen).\n";
        std::cout << "Press any key to return...\n";
        _getch();
        return;
    }

    Screen::displayScreensOptionsMenu(screens);

    std::cout << "\nChoose a screen by number (1 - " << screens.size() << "): ";
    char choice = _getch();
    size_t index = choice - '1';

    if (index < screens.size()) {
        selectedScreenFile = SCREENS_DIR + screens[index].name;
        std::cout << "\nScreen selected: " << selectedScreenFile << "\n";
    }
    else {
        std::cout << "\nInvalid choice. Returning to menu.\n";
    }

    std::cout << "\nPress any key to continue...\n";
    _getch();
}

// GPT prompt - summarize me the game rules
void MenuManager::showInstructionsAndKeys() {
    std::cout << "==================== TANK BATTLE - GAME INSTRUCTIONS ====================\n\n";

    std::cout << ">> OBJECTIVE <<\n";
    std::cout << "Each player controls a tank inside a maze. Tanks can move, rotate, and shoot.\n";
    std::cout << "The goal is to destroy the other tank using shells or mines.\n";
    std::cout << "Avoid walls and mines! Touching them will end the game.\n\n";

    std::cout << ">> MOVEMENT CONTROLS <<\n";
    std::cout << "Each tank has 2 tracks. You control each track separately.\n";
    std::cout << "Movement continues automatically until 'STAY' is pressed.\n\n";

    std::cout << "Player 1 Controls:\n";
    std::cout << "  Q - Left Track Forward    A - Left Track Backward\n";
    std::cout << "  E - Right Track Forward   D - Right Track Backward\n";
    std::cout << "  S - Stay                  W - Shoot\n\n";

    std::cout << "Player 2 Controls:\n";
    std::cout << "  U - Left Track Forward    J - Left Track Backward\n";
    std::cout << "  O - Right Track Forward   L - Right Track Backward\n";
    std::cout << "  K - Stay                  I - Shoot\n\n";

    std::cout << ">> ROTATION LOGIC <<\n";
    std::cout << "  - Right track forward only     -> Rotate counter-clockwise (8 steps)\n";
    std::cout << "  - Right forward + Left back    -> Rotate counter-clockwise (4 steps)\n";
    std::cout << "  - Left track forward only      -> Rotate clockwise (8 steps)\n";
    std::cout << "  - Left forward + Right back    -> Rotate clockwise (4 steps)\n\n";

    std::cout << ">> GAME ELEMENTS <<\n";
    std::cout << "  Tank:          O\n";
    std::cout << "  Cannon:        | / \\ - (depends on direction)\n";
    std::cout << "  Wall:          # (takes 2 hits to destroy)\n";
    std::cout << "  Mine:          @\n";
    std::cout << "  Shell:         *\n\n";

    std::cout << ">> GAME RULES <<\n";
    std::cout << "- Tanks die if hit by a shell or step on a mine.\n";
    std::cout << "- Shells can destroy other shells on contact.\n";
    std::cout << "- Cannon hit - tank survives but loses cannon.\n";
    std::cout << "- Mines are hidden under cannons.\n";
    std::cout << "- Walls fall after 2 shell hits.\n";
    std::cout << "- Map boundaries wrap around (teleport to other side).\n";
    std::cout << "- After shooting, a 5-cycle cooldown is applied.\n\n";

    std::cout << ">> PAUSING <<\n";
    std::cout << "- Press ESC to pause.\n";
    std::cout << "- While paused: Press ESC again to resume, or X to return to the main menu.\n\n";

    std::cout << "=========================================================================\n";
    std::cout << "Press any key to return to the menu...\n";
    _getch();
}
