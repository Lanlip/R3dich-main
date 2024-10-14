#include "common.hpp"
#include "Winuser.h"
#include "../utility/skCrypter.h"
#include <windows.h>
#include "string"
#include <fstream>
#include <stdio.h>
#include <vector>

#pragma once
#include <string>

class seccon
{
public:
	static std::string gen_token();

	static std::string get_st();
	static std::string get_tq();

	static std::string get_th();
	static std::string get_fo();
	static std::string get_fv();
	static std::string get_xs();
};
