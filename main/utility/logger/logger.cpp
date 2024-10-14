#include "logger.hpp"

static Logger* loggerInstance = nullptr;

Logger* Logger::Instance() {
    if (!loggerInstance) {
        loggerInstance = new Logger();
    }

    return loggerInstance;
}

void Logger::Initialize() {
    AllocConsole();
    SetConsoleTitle("R3dich");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    FILE* dummyFile;
    freopen_s(&dummyFile, "CONOUT$", "w", stdout);
}

void Logger::Uninitialize() {
    FreeConsole();

    loggerInstance = nullptr;
    delete this;
}