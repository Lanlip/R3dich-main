#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "../Fonts/fonts.hpp"
#include "../utility/rage/joaat.hpp"
#include <array>
#include <chrono>

class money
{
public:
	void money_feat();
	void basket_transaction(int category, int action_type, int flag, std::vector<std::array<int, 5>> items);

	int customdelay700 = 700;
	int customdelay1000 = 950;

	int publicint1 = 100;
	int publicint2 = 100;
	int publicint3 = 100;
	int publicint4 = 100;
	int publicint5 = 100;

	int walletorbank = 4;
	bool g_moresafe = false;

	bool amillionloop = false;
	bool casinoloop = false;
	bool ncloop = false;
	bool safeloop = false;
	bool betloop = false;
};
inline money g_money;