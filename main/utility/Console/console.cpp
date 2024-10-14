#include "console.hpp"
#include <iostream>

Console g_console;

LRESULT CALLBACK Console::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCHITTEST) {
        return HTCAPTION;
    }
    return CallWindowProc((WNDPROC)((Console*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->originalWndProc, hwnd, uMsg, wParam, lParam);
}

Console::Console() : consoleWindow(nullptr), originalWndProc(0) {}

Console::~Console() {
    if (consoleWindow && originalWndProc) {
        SetWindowLongPtr(consoleWindow, GWLP_WNDPROC, originalWndProc);
    }
}

bool Console::Initialize() {
    consoleWindow = GetConsoleWindow();
    if (!consoleWindow) {
        std::cerr << "Failed to get console window handle." << std::endl;
        return false;
    }

    SetWindowLongPtr(consoleWindow, GWLP_USERDATA, (LONG_PTR)this);
    originalWndProc = SetWindowLongPtr(consoleWindow, GWLP_WNDPROC, (LONG_PTR)WindowProc);

    DWORD consoleMode;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleMode(hConsole, &consoleMode)) {
        SetConsoleMode(hConsole, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    return true;
}

bool Console::SetFont(int width, int height) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = width;
    cfi.dwFontSize.Y = height;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");
    if (!SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi)) {
        std::cerr << "Failed to set console font. Error: " << GetLastError() << std::endl;
        return false;
    }
    return true;
}

bool Console::SetOpacity(BYTE opacity) {
    if (!SetLayeredWindowAttributes(consoleWindow, 0, opacity, LWA_ALPHA)) {
        std::cerr << "Failed to set opacity. Error: " << GetLastError() << std::endl;
        return false;
    }
    return true;
}

bool Console::EnableTransparency() {
    LONG style = GetWindowLong(consoleWindow, GWL_EXSTYLE);
    if (!SetWindowLong(consoleWindow, GWL_EXSTYLE, style | WS_EX_LAYERED)) {
        std::cerr << "Failed to set layered window style. Error: " << GetLastError() << std::endl;
        return false;
    }
    return SetLayeredWindowAttributes(consoleWindow, 0, 255, LWA_ALPHA);
}

void Console::SetTextColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}