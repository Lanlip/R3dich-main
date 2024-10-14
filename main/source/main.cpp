#include "main.hpp"
#include "../secure_contents/securecon.hpp"
#include "../secure_contents/Req/secureget.hpp"
#include "../utility/logger/logger.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/config/config.hpp"
#include "../Feat/Recovery/Recovery.hpp"
#include "../Feat/Misc/Misc.hpp"
#include "script/manager.hpp"
#include "script/script.hpp"
#include "hooks/hooks.hpp"
#include "gui/gui.hpp"
#include "features/features.hpp"
#include "Auth/Auth.hpp"
#include "../Feat/Self/Self.hpp"
#include "common.hpp"
#include "Winuser.h"
#include "../utility/skCrypter.h"
#include <windows.h>
#include "string"
#include <fstream>
#include <stdio.h>
#include <vector>

#include "../utility/Overlay/overlay.hpp"
#include "../utility/Console/console.hpp"

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__
#define MY_COMPILATION_TIME skCrypt("2024-07-28 14:30:00");

extern std::string securekey = "";
extern std::string securekey_encode = "";
extern std::string external_ip = "";
extern std::string external_ip_encode = "";

extern std::string key = "";
static HANDLE coreThread;
static DWORD lpThreadId = 0;
wchar_t wideMessage[256];
bool logged_in = false;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string Base64Encode(const std::string& input) {
    std::string output;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Base64Decode(const std::string& input) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

    std::string output;
    std::vector<int> buffer(4);
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}

std::string getLast6Charsonmain(const std::string& input) {
    return input.substr(input.length() - 6);
}

void RPCScript() {
    while (true) {
        if (*Signatures::Instance()->definitions.gameState == 0) {
            g_feat.DiscordInit();
            Queue::Instance()->Tick();
        }

        Script::Current()->ScriptYield();
    }
}

void FeaturesScript() {
    while (true) {
        if (*Signatures::Instance()->definitions.gameState == 0) {
            Features::Instance()->Tick();
            Queue::Instance()->Tick();
        }

        Script::Current()->ScriptYield();
    }
}

void RecoveryScript() {
    while (true) {
        if (*Signatures::Instance()->definitions.gameState == 0) {
            g_money.money_feat();
            Queue::Instance()->Tick();
        }

        Script::Current()->ScriptYield();
    }
}

void SelfScript() {
    while (true) {
        if (*Signatures::Instance()->definitions.gameState == 0) {
            g_self.self_feattt();
            Queue::Instance()->Tick();
        }

        Script::Current()->ScriptYield();
    }
}

void MiscScript() {
    while (true) {
        if (*Signatures::Instance()->definitions.gameState == 0) {
            g_misc.misc_feat();
            Queue::Instance()->Tick();
        }

        Script::Current()->ScriptYield();
    }
}

void setConsoleTextColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

std::string getLastIP(const std::string& ip) {
    if (ip.length() < 5) {
        return ip;
    }
    return ip.substr(ip.length() - 5);
}

DWORD WINAPI Core(LPVOID lpParam) {

    /*
    if (!g_console.Initialize()) {
        std::cerr << "Failed to Initializ." << std::endl;
        return 1;
    }

    if (!g_console.EnableTransparency()) {
        std::cerr << "Failed to enable transparency" << std::endl;
    }

    if (!g_console.SetOpacity(200)) {
        std::cerr << "Failed to Set opacity" << std::endl;
    }

    if (!g_console.SetFont(0, 16)) {
        std::cerr << "Failed to Set font" << std::endl;
    }
    */

    Logger::Instance()->Initialize();
    setConsoleTextColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printf(skCrypt(R"(
__________________      .___.__       .__      ___________           .__   
\______   \_____  \   __| _/|__| ____ |  |__   \__    ___/___   ____ |  |  
 |       _/ _(__  <  / __ | |  |/ ___\|  |  \    |    | /  _ \ /  _ \|  |  
 |    |   \/       \/ /_/ | |  \  \___|   Y  \   |    |(  <_> |  <_> )  |__
 |____|_  /______  /\____ | |__|\___  >___|  /   |____| \____/ \____/|____/
        \/       \/      \/         \/     \/                              
============================================================================ WELCOME TO R3DICH RECOVERY TOOL !!
)"));
    printf(skCrypt("============================================================================ BUILD INFO: %s"), Version);
    printf(skCrypt("\n"));
    setConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    Logger::Instance()->Log(skCrypt("Info"), skCrypt("Official Website : Redich.cc"));

    Logger::Instance()->Log(skCrypt("Authentication"), skCrypt("Finding Local Authentication Data..."));
    const char* appData = std::getenv("appdata");
    if (appData)
    {
        std::string filePath = appData + std::string("\\R3dich\\Auth.ini");

        std::ifstream iniFile(filePath);

        if (iniFile.is_open())
        {
            Logger::Instance()->Log(skCrypt("Authentication"), skCrypt("Authentication Data Found :)"));
            std::string line;
            while (std::getline(iniFile, line))
            {
                if (line.find(skCrypt("Key=")) != std::string::npos)
                {
                    key = line.substr(4);
                    break;
                }
            }
            iniFile.close();
        }
        else
        {
            Logger::Instance()->Log(skCrypt("ERROR"), skCrypt("Local Authentication data does not exist :("));
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
            running = false;
        }
    }
    else
    {
        Logger::Instance()->Log(skCrypt("ERROR"), skCrypt("FATAL Error :("));
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
        running = false;
    }

    Logger::Instance()->Log(skCrypt("Requesting"), skCrypt("Requesting Authentication..."));

    auth::LoginStatus status = auth::login(key);


    securekey = getLast6Charsonmain(key);
    securekey_encode = Base64Encode(securekey);

    if (status == auth::LoginStatus::LOGIN_TRUE)
    {
        Logger::Instance()->Log(skCrypt("Authentication"), skCrypt("Passed all authentication steps successfully :)"));
        logged_in = true;
        if (logged_in)
        {
            Logger::Instance()->Log(skCrypt("Requesting"), skCrypt("Requesting Secure Contents..."));
            seccon::gen_token();

            seccon::get_st();
            Logger::Instance()->Log(skCrypt("Debug"), skCrypt("Stealth Content Initialized"));
            seccon::get_tq();
            Logger::Instance()->Log(skCrypt("Debug"), skCrypt("Transaction Content Initialized"));

            seccon::get_th();
            Logger::Instance()->Log(skCrypt("Debug"), skCrypt("Partner Panel Content Initialized"));
            seccon::get_fo();
            Logger::Instance()->Log(skCrypt("Debug"), skCrypt("Main Panel Content Initialized"));
            seccon::get_fv();
            Logger::Instance()->Log(skCrypt("Debug"), skCrypt("Hotkey#1 Content Initialized"));
            seccon::get_xs();
            Logger::Instance()->Log(skCrypt("Debug"), skCrypt("Hotkey#2 Content Initialized"));


            Logger::Instance()->Log(skCrypt("Requesting"), skCrypt("Requesting additional user information..."));
            external_ip = auth::getExternalIP();
            external_ip_encode = getLastIP(external_ip);

            Logger::Instance()->Log(skCrypt("User Information"), skCrypt("Personal identification Key Seed: %s"), securekey);
            Logger::Instance()->Log(skCrypt("User Information"), skCrypt("Personal identification Key: %s"), securekey_encode);
            Logger::Instance()->Log(skCrypt("User Information"), skCrypt("Personal identification Adress: %s"), external_ip_encode);
        }
    }

    else if (status == auth::LoginStatus::LOGIN_FALSE)
    {
        Logger::Instance()->Log(skCrypt("ERROR"), ("Authentication Error, Invaild Credentials :("));
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
        running = false;
    }

    else if (status == auth::LoginStatus::LOGIN_FALSE_IPLOCKED)
    {
        Logger::Instance()->Log(skCrypt("ERROR"), skCrypt("Authentication Error, Unknown IP Address (IP-Locked) :("));
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
        running = false;
    }

    else if (status == auth::LoginStatus::LOGIN_FALSE_ERROR)
    {
        Logger::Instance()->Log(skCrypt("ERROR"), skCrypt("Authentication Error, Unknown Error :("));
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
        running = false;
    }

    if (logged_in)
    {
        Logger::Instance()->Log(skCrypt("Requesting"), skCrypt("Requesting User Permissions..."));

        auth::Level keylevel = auth::get_level(key);

        if (keylevel == auth::Level::LEVEL_PARTNER)
        {
            g_partner = true;
            Logger::Instance()->Log(skCrypt("Authentication"), skCrypt("You are Official Partner :)"));
        }
        else if (keylevel == auth::Level::LEVEL_NORMAL)
        {
            g_partner = false;
            Logger::Instance()->Log(skCrypt("Authentication"), skCrypt("You are Normal User :)"));
        }
        else
        {
            Logger::Instance()->Log(skCrypt("ERROR"), skCrypt("Unknown Error :("));
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
            running = false;
        }
    }

    if (logged_in)
    {
        ScriptManager::Instance()->Add(std::make_unique<Script>(&FeaturesScript), skCrypt("Basic Script is Running!"));
        ScriptManager::Instance()->Add(std::make_unique<Script>(&SelfScript), skCrypt("Self Script is Running!"));
        ScriptManager::Instance()->Add(std::make_unique<Script>(&RecoveryScript), skCrypt("Recovery Script is Running!"));
        ScriptManager::Instance()->Add(std::make_unique<Script>(&MiscScript), skCrypt("Misc Script is Running!"));
        Logger::Instance()->Log(skCrypt("Script"), skCrypt("Scripts Initialized"));

        Signatures::Instance()->Initialize();
        Features::Instance()->Initialize();
        Hooks::Instance()->Initialize();
        g_feat.DiscordInit();
        Logger::Instance()->Log(skCrypt("Discord"), skCrypt("Rich Presence Initialized"));
        Logger::Instance()->Log(skCrypt("WELCOME"), skCrypt("Welcome to R3dich, Open with F4"));
        Logger::Instance()->Log(skCrypt("WARNING"), skCrypt("THIS IS NOT ERROR: Due to an internal issue, it is only available in English instead of Korean"));
        Logger::Instance()->Log(skCrypt("Hotkeys"), skCrypt("F6 = Join Invite lobby | F5 = Noclip on/off | F4 = Menu on/off | F3 = Partner Panel on/off"));
        //Gui::Instance()->AddNotification(reinterpret_cast<const char*>((u8"내부적 문제로 인해 레디치 메뉴 언어는 '한국어' 가 아닌 '영어'로 고정되며\n한국어로 변경할수 있는 방법은 없습니다.\n언어 설정 관련하여 커뮤니티에서 질문시 해당 커뮤니티에서 불이익을 받을수 있습니다.\n\n참고: 이것은 오류가 아니며 확인버튼을 누른후 게임과 핵이 모두 정상적으로 작동할것입니다.")));
        ShowCenterMessage("Redich is here :) Open with F4", 7.0f);
        while (running) {

        }
        Hooks::Instance()->Uninitialize();
        Gui::Instance()->Uninitialize();
        Signatures::Instance()->Uninitialize();
        Features::Instance()->Uninitialize();
        Config::Instance()->Uninitialize();
        ScriptManager::Instance()->Uninitialize();
        Queue::Instance()->Uninitialize();
        Logger::Instance()->Log(skCrypt("Unload"), skCrypt("Farewell, You can now close this window!"));
        Logger::Instance()->Uninitialize();
        g_feat.DiscordDestroy();
        CloseHandle(coreThread);
        FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        coreThread = CreateThread(
            nullptr,
            0,
            Core,
            hinstDLL,
            0,
            &lpThreadId
        );
    }

    return TRUE;
}