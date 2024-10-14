#pragma once
#include <string>

class auth
{
public:
    enum class LoginStatus
    {
        LOGIN_TRUE,
        LOGIN_FALSE,
        LOGIN_FALSE_IPLOCKED,
        LOGIN_FALSE_ERROR
    };

    enum class Level
    {
        LEVEL_PARTNER,
        LEVEL_NORMAL
    };

    static LoginStatus login(const std::string& id);
    static Level get_level(const std::string& id);
    static std::string getExternalIP();
};
