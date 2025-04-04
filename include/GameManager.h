#pragma once
#include <iostream>
#define FRAME_RATE 1000 // milliseconds
#define ESCAPE 27

class GameManager {
public:
    GameManager();          

    void openMenu();
    void startGame();
    void pauseGame();
    void resumeGame();
    void update();
    void render();
    void gameOver();

private:
    bool isRunning;
    bool isPaused;

    void showInstructionsAndKeys();
};
