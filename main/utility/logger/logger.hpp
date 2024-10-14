#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <common.hpp>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DISCORD_BLURPLE RGB(114, 137, 217)  // RGB(0x72, 0x89, 0xd9)

class Logger {
public:
    static Logger* Instance();
    void Initialize();
    void Uninitialize();
    void Log(const char* format, const char* raw, ...) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        va_list args;
        va_start(args, raw);
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf("[ LOG ]::");
        if (strcmp(format, "ERROR") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        }
        else if (strcmp(format, "Debug") == 0 || strcmp(format, "User Information") == 0 || strcmp(format, "Info") == 0 || strcmp(format, "Hotkeys") == 0 || strcmp(format, "Script") == 0 || strcmp(format, "Signature") == 0 || strcmp(format, "Hooks") == 0 ) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
        }
        else if (strcmp(format, "Authentication") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        }
        else if (strcmp(format, "WARNING") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        }
        else if (strcmp(format, "Requesting") == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }
        else if (strcmp(format, "Discord") == 0) {
            SetDiscordColor(hConsole);
        }
        else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        }
        printf("[%s] ", format);
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        char combined[1024];
        vsprintf(combined, raw, args);
        printf("%s\n", combined);
        va_end(args);
    }

private:
    void SetDiscordColor(HANDLE hConsole) {
        static bool isDiscordColorSet = false;
        if (!isDiscordColorSet) {
            CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
            csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
            GetConsoleScreenBufferInfoEx(hConsole, &csbiex);
            csbiex.ColorTable[1] = DISCORD_BLURPLE;
            SetConsoleScreenBufferInfoEx(hConsole, &csbiex);
            isDiscordColorSet = true;
        }
        SetConsoleTextAttribute(hConsole, 1);
    }
};

#endif