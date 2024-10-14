#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "../Fonts/fonts.hpp"
#include "../utility/rage/joaat.hpp"
#include "../script_global/script_global.hpp"

#include "Misc.hpp"

void misc::misc_feat()
{
	if (g_misc.bypasserror)
	{
		*script_global(4537455).as<int*>() = 0;
	}

	if (g_misc.noidlecam)
	{
		CAM::INVALIDATE_CINEMATIC_VEHICLE_IDLE_MODE();
		CAM::INVALIDATE_IDLE_CAM();
	}

	if (g_misc.showbalance)
	{
		HUD::SET_MULTIPLAYER_BANK_CASH();
		HUD::SET_MULTIPLAYER_WALLET_CASH();
	}

	if (g_misc.busyspin)
	{
		HUD::BUSYSPINNER_OFF();
	}

	if (g_misc.esp_box)
	{
		
		for (int y = 0; y < NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS(); y++)
		{
			NativeVector handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(y), 0, 0, 0);

			GRAPHICS::DRAW_LINE(handleCoords.x + 0.3, handleCoords.y + 0.3, handleCoords.z + 0.75, handleCoords.x + 0.3, handleCoords.y - 0.3, handleCoords.z + 0.75, 255, 255, 0, 255); // top Box
			GRAPHICS::DRAW_LINE(handleCoords.x + 0.3, handleCoords.y - 0.3, handleCoords.z + 0.75, handleCoords.x - 0.3, handleCoords.y - 0.3, handleCoords.z + 0.75, 255, 255, 0, 255);
			GRAPHICS::DRAW_LINE(handleCoords.x - 0.3, handleCoords.y - 0.3, handleCoords.z + 0.75, handleCoords.x - 0.3, handleCoords.y + 0.3, handleCoords.z + 0.75, 255, 255, 0, 255);
			GRAPHICS::DRAW_LINE(handleCoords.x - 0.3, handleCoords.y + 0.3, handleCoords.z + 0.75, handleCoords.x + 0.3, handleCoords.y + 0.3, handleCoords.z + 0.75, 255, 255, 0, 255);

			GRAPHICS::DRAW_LINE(handleCoords.x + 0.3, handleCoords.y + 0.3, handleCoords.z - 0.75, handleCoords.x + 0.3, handleCoords.y - 0.3, handleCoords.z - 0.75, 255, 255, 0, 255); // bottom Box
			GRAPHICS::DRAW_LINE(handleCoords.x + 0.3, handleCoords.y - 0.3, handleCoords.z - 0.75, handleCoords.x - 0.3, handleCoords.y - 0.3, handleCoords.z - 0.75, 255, 255, 0, 255);
			GRAPHICS::DRAW_LINE(handleCoords.x - 0.3, handleCoords.y - 0.3, handleCoords.z - 0.75, handleCoords.x - 0.3, handleCoords.y + 0.3, handleCoords.z - 0.75, 255, 255, 0, 255);
			GRAPHICS::DRAW_LINE(handleCoords.x - 0.3, handleCoords.y + 0.3, handleCoords.z - 0.75, handleCoords.x + 0.3, handleCoords.y + 0.3, handleCoords.z - 0.75, 255, 255, 0, 255);

			GRAPHICS::DRAW_LINE(handleCoords.x + 0.3, handleCoords.y + 0.3, handleCoords.z - 0.75, handleCoords.x + 0.3, handleCoords.y + 0.3, handleCoords.z + 0.75, 255, 255, 0, 255); // bottom Box
			GRAPHICS::DRAW_LINE(handleCoords.x + 0.3, handleCoords.y - 0.3, handleCoords.z - 0.75, handleCoords.x + 0.3, handleCoords.y - 0.3, handleCoords.z + 0.75, 255, 255, 0, 255);
			GRAPHICS::DRAW_LINE(handleCoords.x - 0.3, handleCoords.y - 0.3, handleCoords.z - 0.75, handleCoords.x - 0.3, handleCoords.y - 0.3, handleCoords.z + 0.75, 255, 255, 0, 255);
			GRAPHICS::DRAW_LINE(handleCoords.x - 0.3, handleCoords.y + 0.3, handleCoords.z - 0.75, handleCoords.x - 0.3, handleCoords.y + 0.3, handleCoords.z + 0.75, 255, 255, 0, 255);
		}
		
	}

	if (g_misc.esp_line)
	{
		for (int i = 0; i < NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS(); i++)
		{
			NativeVector locationOne = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false);
			NativeVector locationTwo = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
			GRAPHICS::DRAW_LINE(locationOne.x, locationOne.y, locationOne.z, locationTwo.x, locationTwo.y, locationTwo.z, 255, 255, 0, 255);
		}
		
	}

	if (g_misc.esp_name)
	{
		/*
		for (int i = 0; i < NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS(); i++)
		{
			float screenX;
			float screenY;
			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 0, 0, 0);
			Player player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
			if (ENTITY::DOES_ENTITY_EXIST(player))
			{
				BOOL screenCoord = GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(coords.x, coords.y, coords.z, &screenX, &screenY);
				HUD::SET_TEXT_FONT(7);
				HUD::SET_TEXT_SCALE(0.0, 0.30);
				HUD::SET_TEXT_COLOUR(255, 255, 255, 125);
				HUD::SET_TEXT_CENTRE(0);
				HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
				HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
				HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
				HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i)));
				HUD::END_TEXT_COMMAND_DISPLAY_TEXT(screenX, screenY, 1);
				HUD::SET_TEXT_OUTLINE();
			}
		}
		*/
	}
}