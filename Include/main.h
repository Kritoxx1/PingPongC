#ifndef MAIN_H
#define MAIN_H

/*INCLUDES*/
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

struct Window {
    HWND hwnd;
    HINSTANCE hInstance;
    WNDCLASS wc;
    int nCmdShow;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void WindowCycle();

#endif //MAIN_H
