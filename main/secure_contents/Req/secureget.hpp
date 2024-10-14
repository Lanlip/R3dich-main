#pragma once
#include <string>

class secget
{
public:

    enum class Token
    {
        TOKEN_TRUE,
        TOKEN_FALSE,
        TOKEN_ERROR
    };

    static Token get_token(const std::string& id);
    static std::string Get(const std::string& url);
};