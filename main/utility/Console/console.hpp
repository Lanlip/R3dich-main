#pragma once
#include <Windows.h>

class Console {
private:
    HWND consoleWindow;
    LONG_PTR originalWndProc;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    Console();
    ~Console();

    bool SetFont(int width = 0, int height = 16);
    bool SetOpacity(BYTE opacity);
    bool EnableTransparency();
    void SetTextColor(WORD color);
    bool Initialize();
};

extern Console g_console;