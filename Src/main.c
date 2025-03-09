#include "../Include/main.h"

#include <pthread.h>
#include <stdbool.h>

#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH  50
#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600
#define BALL_SIZE  30
#define MOVE_SPEED 10
#define BALL_SPEED_X 5
#define BALL_SPEED_Y 5

int player1_posY = 200;
int player2_posY = 200;

struct Ball {
    int x, y;
    int speedX, speedY;
} ball = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, BALL_SPEED_X, BALL_SPEED_Y};

struct Window window;

DWORD WINAPI player1_movement(LPVOID param) {
    HWND hwnd = (HWND)param;
    while (true) {
        if (GetAsyncKeyState(VK_UP) & 0x8000 && player1_posY > 0) {
            player1_posY -= MOVE_SPEED;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000 && player1_posY < (WINDOW_HEIGHT - PADDLE_HEIGHT)) {
            player1_posY += MOVE_SPEED;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        Sleep(16);
    }
    return 0;
}

/* Thread-Funktion für Spieler 2 */
DWORD WINAPI player2_movement(LPVOID param) {
    HWND hwnd = (HWND)param;
    while (true) {
        if (GetAsyncKeyState(0x57) & 0x8000 && player2_posY > 0) { // W-Taste
            player2_posY -= MOVE_SPEED;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        if (GetAsyncKeyState(0x53) & 0x8000 && player2_posY < (WINDOW_HEIGHT - PADDLE_HEIGHT)) { // S-Taste
            player2_posY += MOVE_SPEED;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        Sleep(16);
    }
    return 0;
}

DWORD WINAPI ball_movement(LPVOID param) {
    HWND hwnd = (HWND)param;
    while (true) {
        ball.x += ball.speedX;
        ball.y += ball.speedY;

        if (ball.y <= 0 || ball.y + BALL_SIZE >= WINDOW_HEIGHT) {
            ball.speedY = -ball.speedY;
        }

        if ((ball.x <= 150 && ball.x >= 100 && ball.y >= player1_posY && ball.y <= player1_posY + PADDLE_HEIGHT) ||
             (ball.x >= 850 && ball.x <= 900 && ball.y >= player2_posY && ball.y <= player2_posY + PADDLE_HEIGHT)) {
            ball.speedX = -ball.speedX;
        }

        if (ball.x < 0 || ball.x > WINDOW_WIDTH) {
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            ball.speedX = (ball.speedX > 0) ? -BALL_SPEED_X : BALL_SPEED_X;  // Change direction on reset
        }

        /* Refresh window */
        InvalidateRect(hwnd, NULL, TRUE);
        Sleep(16);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
        return 0;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            /*Player Rectangles*/
            Rectangle(hdc, 100, player1_posY, 150, player1_posY + 100); // Player 1
            Rectangle(hdc, 850, player2_posY, 900, player2_posY + 100); // Player 2

            /*Ball*/
            Ellipse(hdc, ball.x, ball.y, ball.x + BALL_SIZE, ball.y + BALL_SIZE);

            EndPaint(hwnd, &ps);
        break;

    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

void WindowCycle() {
    MSG msg = {};
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main(void) {
    window.nCmdShow = SW_SHOW;
    window.wc.hInstance = GetModuleHandle(NULL);
    window.wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    window.wc.lpfnWndProc = WndProc;
    window.wc.lpszClassName = TEXT("windowClass");

    if (!RegisterClass(&window.wc)) {
        perror("RegisterClass failed (GAME)\n");
        return 1;
    }

    window.hwnd = CreateWindow(
        (LPSTR)window.wc.lpszClassName,
        TEXT("Window"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1000, 600,
        NULL,
        NULL,
        window.hInstance,
        NULL);

    if (!window.hwnd) {
        perror("CreateWindow failed (GAME)\n");
        return 1;
    }

    ShowWindow(window.hwnd, window.nCmdShow);
    UpdateWindow(window.hwnd);

    CreateThread(NULL, 0, player1_movement, (LPVOID)window.hwnd, 0, NULL);
    CreateThread(NULL, 0, player2_movement, (LPVOID)window.hwnd, 0, NULL);
    CreateThread(NULL, 0, ball_movement, (LPVOID)window.hwnd, 0, NULL);

    WindowCycle();

    return 0;
}