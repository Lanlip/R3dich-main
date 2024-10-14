#include "Recovery.hpp"
#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "../Fonts/fonts.hpp"
#include "../source/gui/gui.hpp"
//#include "../script_global/script_global.hpp"
#include "../utility/rage/joaat.hpp"
#include "../utility/skCrypter.h"
#include <array>
#include <chrono>

/// /////////////

int spam_event_time = 0;
static int value = NULL;

bool flagone = true;
bool flagtwo = true;
bool ncfirst = true;
bool tlag = true;
bool nc_real_first = true;

void money::basket_transaction(int category, int action_type, int flag, std::vector<std::array<int, 5>> items)
{
	if (NETSHOPPING::NET_GAMESERVER_BASKET_IS_ACTIVE())
		NETSHOPPING::NET_GAMESERVER_BASKET_END();

	int transaction = -1;
	__int64 queue = *Signatures::Instance()->definitions.transactionQueue;

	if (Signatures::Instance()->definitions.constructBasket(queue, &transaction, category, action_type, flag)) {

		for (auto& item : items) {
			Signatures::Instance()->definitions.addItemToBasket(queue, item.data());
		}
		Signatures::Instance()->definitions.processTransaction(queue, transaction, Secure69420);
		//Signatures::Instance()->definitions.processTransaction(queue, transaction, 69420);
	}
}

void money::money_feat()
{

	if (g_money.betloop)
	{
		g_watermark = true;
		if (g_moresafe)
		{
			if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay700)
			{
				g_money.basket_transaction(1474183246, 1445302971, g_money.walletorbank,
					{
						 {(int)2896648878, -1, MISC::GET_RANDOM_INT_IN_RANGE(400000, 500000), 0, Secure1}
					});
				spam_event_time = MISC::GET_GAME_TIMER();
			}
		}
		else
		{
			if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay700)
			{
				g_money.basket_transaction(1474183246, 1445302971, g_money.walletorbank,
					{
						 {(int)2896648878, -1, 500000, 0, Secure1}
					});
				spam_event_time = MISC::GET_GAME_TIMER();
			}
		}
	}

	if (g_money.safeloop)
	{
		g_watermark = true;
		if (g_moresafe)
		{
			if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay700)
			{
				g_money.basket_transaction(1474183246, 1445302971, g_money.walletorbank,
					{
						 {1628412596, -1, MISC::GET_RANDOM_INT_IN_RANGE(40000, 50000), 0, 1}
					});
				spam_event_time = MISC::GET_GAME_TIMER();
			}
		}
		else
		{
			if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay700)
			{
				g_money.basket_transaction(1474183246, 1445302971, g_money.walletorbank,
					{
						 {1628412596, -1, 50000, 0, 1}
					});
				spam_event_time = MISC::GET_GAME_TIMER();
			}
		}
	}

	if (g_money.amillionloop)
	{
		g_watermark = true;
		if (g_moresafe)
		{
			if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay700)
			{
				g_money.basket_transaction(1474183246, 1445302971, g_money.walletorbank,
					{
						 {1633116913, -1, MISC::GET_RANDOM_INT_IN_RANGE(150000, 180000), 0, 1}
					});
				spam_event_time = MISC::GET_GAME_TIMER();
			}
		}
		else
		{
			if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay700)
			{
				g_money.basket_transaction(1474183246, 1445302971, g_money.walletorbank,
					{
						 {1633116913, -1, 180000, 0, 1}
					});
				spam_event_time = MISC::GET_GAME_TIMER();
			}
		}
	}

	if (g_money.ncloop)
	{
		g_watermark = true;
		if ((MISC::GET_GAME_TIMER() - spam_event_time) > g_money.customdelay1000)
		{
			if (nc_real_first)
			{
				if (flagtwo && ncfirst)
				{
					Gui::Instance()->AddNotification(skCrypt("Setting up Transaction... Please wait for seconds"));
					g_money.basket_transaction(-1799524201, -1018905335, 8, { {545376149, 129314700, 0, Secure1, Secure1} });
					ncfirst = false;
					flagtwo = false;
				} 
				else if (!flagtwo)
				{
					g_money.basket_transaction(-1799524201, 2108234284, 8, {
							{545376149, 129314700, 0, Secure1, Secure1},
							{-544423649, -1, 0, 0, 50000},
							{1435847077, -544423649, 0, 50000, Secure1}
						});
					ncfirst = true;
					flagtwo = true;
					nc_real_first = false;
					//ImGui::InsertNotification({ ImGuiToastType_Success, 5000, u8"스텔스 루프#3 사용을 위하여 나이트클럽의 금고 수익을 조정하였습니다.\n만약 플레이에 문제가 생긴다면 나이트클럽 금고 비우기 기능을 통해\n수동으로 수익을 제거할수 있습니다." });
				}
			}
			else if (g_moresafe)
			{
				if (flagtwo && ncfirst)
				{
					g_money.basket_transaction(-1799524201, -1018905335, 8, { {545376149, -620977285, 0, Secure1, Secure1} });
					ncfirst = false;
					flagtwo = false;
				}
				else if (!flagtwo)
				{
					g_money.basket_transaction(-1799524201, -788753717, g_money.walletorbank, {
						{ 545376149, -620977285, 0, Secure1, Secure1 },
						{ -544423649, -1, 0, Secure1, Secure1 },
						{ 1783677420, -544423649, MISC::GET_RANDOM_INT_IN_RANGE(5800000, 6000000), 0, Secure1 }
						});
					ncfirst = true;
					flagtwo = true;
				}
			}
			else
			{
				if (flagtwo && ncfirst)
				{
					g_money.basket_transaction(-1799524201, -1018905335, 8, { {545376149, -620977285, 0, Secure1, Secure1} });
					ncfirst = false;
					flagtwo = false;
				}
				else if (!flagtwo)
				{
					g_money.basket_transaction(-1799524201, -788753717, g_money.walletorbank, {
						{ 545376149, -620977285, 0, Secure1, Secure1 },
						{ -544423649, -1, 0, Secure1, Secure1 },
						{ 1783677420, -544423649, 6000000, 0, Secure1 }
						});
					ncfirst = true;
					flagtwo = true;
				}
			}
			spam_event_time = MISC::GET_GAME_TIMER();
		}
	}
}