#include "../source/features/features.hpp"
#include "discord_rpc.h"
#include <string>

#pragma warning(disable: 1104)
#pragma comment(lib, "discord-rpc.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Normaliz.lib")

void Features::DiscordInit()
{
	if (discord_rpc)
	{
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		Discord_Initialize("1238814814822727743", &handlers, TRUE, nullptr);
		UpdatePresence();
	}
	else
	{
		DiscordDestroy();
	}

}

void Features::UpdatePresence()
{
	if (discord_rpc)
	{
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "The Best Cheat in S.Korea";
		discordPresence.details = "R3dich Recovery Tool";
		discordPresence.startTimestamp = 0;
		discordPresence.largeImageKey = "logo";
		discordPresence.largeImageText = "By Redich Group";
		discordPresence.smallImageKey = "gtalogo";
		discordPresence.smallImageText = "The Freehack.zip Production";
		Discord_UpdatePresence(&discordPresence);
	}
}

void Features::DiscordDestroy()
{
	Discord_Shutdown();
}