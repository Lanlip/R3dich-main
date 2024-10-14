#include "common.hpp"
#include "Winuser.h"
#include "../utility/skCrypter.h"
#include <windows.h>
#include "string"
#include <fstream>
#include <stdio.h>
#include <vector>
#include "../secure_contents/Req/secureget.hpp"
#include "../secure_contents/securecon.hpp"

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


std::string Base64Decode2(const std::string& input) {
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

extern std::string key;

std::string seccon::gen_token()
{
	secget::get_token(key);
	return key;
}

std::string seccon::get_st()
{
    token_while = false;
    std::string response_st = secget::Get(std::string(skCrypt("https://freehack.zip/api/secure/st")));
    Secure1 = std::stoi(Base64Decode2(response_st));
    return response_st;
}

std::string seccon::get_tq()
{
    std::string response_tq = secget::Get(std::string(skCrypt("https://freehack.zip/api/secure/tq")));
    Secure69420 = std::stoi(Base64Decode2(response_tq));
    return response_tq;
}

std::string seccon::get_th()
{
    std::string response_th = secget::Get(std::string(skCrypt("https://freehack.zip/api/secure/key3")));
    key3 = std::stoi(Base64Decode2(response_th), nullptr, 16);
    return response_th;
}

std::string seccon::get_fo()
{
    std::string response_fo = secget::Get(std::string(skCrypt("https://freehack.zip/api/secure/key4")));
    key4 = std::stoi(Base64Decode2(response_fo), nullptr, 16);
    return response_fo;
}

std::string seccon::get_fv()
{
    std::string response_fv = secget::Get(std::string(skCrypt("https://freehack.zip/api/secure/key5")));
    key5 = std::stoi(Base64Decode2(response_fv), nullptr, 16);
    return response_fv;
}

std::string seccon::get_xs()
{
    std::string response_xs = secget::Get(std::string(skCrypt("https://freehack.zip/api/secure/key6")));
    key6 = std::stoi(Base64Decode2(response_xs), nullptr, 16);
    return response_xs;
}