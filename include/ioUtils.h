#pragma once
#ifndef _IO_UTILS_H_
#define _IO_UTILS_H_

#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>

#include <conio.h>
// _kbhit : Non - blocking function to check if a key has been pressed.
// _getch : Reads a single character from the keyboard without echoing it to the console.


#include <windows.h>
using namespace std;

void gotoxy(int x, int y);
void hideCursor();
void clear_screen();
#endif