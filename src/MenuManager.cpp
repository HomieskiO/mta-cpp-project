#include "MenuManager.h"
#include <conio.h>
#include <windows.h>
#include "IOUtils.h"

MenuManager::MenuManager() {
    gameManager = nullptr;
    coloredGame = true;
}

void MenuManager::openMenu() {
    char choice = 0;

    while (choice != START_GAME && choice != INSTRUCTIONS && choice != EXIT) {
        clearScreen();
        std::cout << "=== Main Menu ===\n";
        std::cout << "(1) Start a new Game\n";
        if (coloredGame) {
            std::cout << "(7) Switch to black and white game\n";
        }
        else {
            std::cout << "(7) Switch to colored game\n";
        }
        std::cout << "(8) Present instructions and keys\n";
        std::cout << "(9) EXIT\n\n";
        std::cout << "Choose an option: ";
        choice = tolower(_getch());

        if (choice == START_GAME) {
            clearScreen();
			gameManager = new GameManager(coloredGame);
            gameManager->startGame();
			delete gameManager;
            choice = 0; // Reset choice to show menu again
        }
        else if (choice == CHANGE_GAME_COLOR) {
            coloredGame = !coloredGame;
        }
        else if (choice == INSTRUCTIONS) {
            clearScreen();
            showInstructionsAndKeys();
            choice = 0; // Reset choice to show menu again
        }
        else if (choice == EXIT) {
            std::cout << "\nExiting game...\n";
            Sleep(FRAME_RATE_MS);
            break;
        }
    }
}

void MenuManager::showInstructionsAndKeys() {
    std::cout << "==================== TANK BATTLE - GAME INSTRUCTIONS ====================\n\n";

    std::cout << ">> OBJECTIVE <<\n";
    std::cout << "Each player controls a tank inside a maze. Tanks can move, rotate, and shoot.\n";
    std::cout << "The goal is to destroy the other tank using shells or mines.\n";
    std::cout << "Avoid walls and mines! Touching them will end the game.\n\n";

    std::cout << ">> MOVEMENT CONTROLS <<\n";
    std::cout << "Each tank has 2 tracks. You control each track separately.\n";
    std::cout << "Movement continues automatically until 'STAY' is pressed.\n";
    std::cout << "Input is case-insensitive.\n\n";

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
    std::cout << "- Cannon hit? Tank survives but loses cannon.\n";
    std::cout << "- Mines are hidden under cannons.\n";
    std::cout << "- Walls fall after 2 shell hits (optional weakened visual).\n";
    std::cout << "- Map boundaries wrap around (teleport to other side).\n";
    std::cout << "- After shooting, a 5-cycle cooldown is applied.\n\n";

    std::cout << ">> PAUSING <<\n";
    std::cout << "- Press ESC to pause.\n";
    std::cout << "- While paused: Press ESC again to resume, or X to return to the main menu.\n\n";

    std::cout << "=========================================================================\n";
    std::cout << "Press any key to return to the menu...\n";
    _getch();
}
