#include "gui.hpp"
#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "../main.hpp"
#include "../Fonts/fonts.hpp"
#include <array>
#include <uchar.h>
#include <cuchar>
//#include "../script_global/script_global.hpp"
#include "../utility/rage/joaat.hpp"
#include "../source/features/features.hpp"
#include "../Comm/comm.hpp"
#include "../Feat/Recovery/Recovery.hpp"
#include "../script/script.hpp"
#include "../Feat//Rank/Rank.hpp"
#include "../Feat/Unlock/Unlock.hpp"
#include "../Feat/Self/Self.hpp"
#include "../Feat/Misc/Misc.hpp"

#include "../utility/skCrypter.h"

#include "../utility/Overlay/overlay.hpp"

using Blip = std::int32_t;
static Gui* guiInstance = nullptr;

Gui* Gui::Instance() {
	if (!guiInstance)
		guiInstance = new Gui();

	int64_t handle = 0;

	return guiInstance;
}

/////////////////
std::string MessageString;

int spam_event = 1;
int spam_event_2 = 1;

bool casino_chips_pass = false;

int test_value = 1;
int rank_value = 1;
int rank_mod = 1;

int rank = 1;
int rp_mode = 1;

int rcwon = 1;
int rclost = 1;
int rcpt = 1;

int dmwon = 1;
int dmlost = 1;
int dmpt = 1;

int jobs = 1;
int betting = 1;
int shared = 1;
int selling_veh = 1;
int goodsp = 1;
int pickedup = 1;

int cashtotale = 1;
int cashtotals = 1;

int totalplay = 1;
int mpplay = 1;

int removedmoney = 1;
int achieve = 1;
/////////////////

enum eScriptGlobals : std::uint32_t
{
	SessionType = 1575035, //2
	InitSession = 1574589, //1
	SessionUNK1 = 32949, //3
	SessionUNK2 = 1574942, //4
	SessionUNK3 = 1575010, //5
	SessionSCTV = 2697731 //SCTV
};

enum ChangeSessionID
{
	GTAO = -1,
	FindPublicSession = 0,
	StartNewPublicSession = 1,
	ClosedCrewSession = 2,
	CrewSession = 3,
	ClosedFriendSession = 6,
	FindFriendsSession = 9,
	StartSoloSession = 10,
	InviteOnlySession = 11,
	JoinCrewSession = 12,
	JoinSCTV = 13,
};

void change_session(ChangeSessionID ID)
{
	*script_global(SessionUNK1).as<int*>() = 0;
	*script_global(SessionType).as<int*>() = ID;
	*script_global(InitSession).at(2).as<int*>() = 0;
	*script_global(SessionSCTV).as<int*>() = 0;
	*script_global(InitSession).as<int*>() = 1;
	*script_global(SessionUNK2).as<int*>() = 0;
	*script_global(SessionUNK3).as<int*>() = -1;
}

int rp_value(int index)
{
	if (index >= 1 && index <= sizeof(levels) / sizeof(levels[0]))
	{
		return levels[index - 1];
	}
	else
	{
		return -1;
	}
}

static const char* Planes1[] = {
	"BESRA", "CARGOPLANE", "CUBAN800", "DODO", "DUSTER",
	"HYDRA", "JET", "LAZER", "LUXOR", "LUXOR2",
	"MAMMATUS", "MILJET", "NIMBUS", "SHAMAL", "STUNT",
	"TITAN", "VELUM", "VELUM2", "VESTRA", "BOMBUSHKA",
	"ALPHAZ1", "TULA", "ROGUE", "PYRO", "MOGUL",
	"NOKOTA", "MOLOTOK", "SEABREEZE", "MICROLIGHT", "HOWARD",
	"STARLING", "AVENGER2", "VOLATOL", "STRIKEFORCE"
};
static const char* Sedans1[] = {
	"ASEA", "ASEA2", "ASTEROPE", "COG55", "COG552",
	"COGNOSCENTI", "COGNOSCENTI2", "EMPEROR", "EMPEROR2", "EMPEROR3",
	"FUGITIVE", "GLENDALE", "INGOT", "INTRUDER", "LIMO2",
	"PREMIER", "PRIMO", "PRIMO2", "REGINA", "ROMERO",
	"SCHAFTER2", "SCHAFTER5", "SCHAFTER6", "STANIER", "STRATUM",
	"STRETCH", "SUPERD", "SURGE", "TAILGATER", "WARRENER",
	"WASHINGTON", "STAFFORD", "NEBULA"
};
static const char* Service1[] = {
	"AIRBUS", "BRICKADE", "BUS", "COACH", "RALLYTRUCK",
	"RENTALBUS", "TAXI", "TOURBUS", "TRASH", "TRASH2",
	"WASTELANDER", "PBUS2"
};
static const char* Sports1[] = {
	"ALPHA", "BANSHEE", "BESTIAGTS", "BLISTA2", "BLISTA3",
	"BUFFALO", "BUFFALO2", "BUFFALO3", "CARBONIZZARE", "COMET2",
	"COQUETTE", "ELEGY", "ELEGY2", "FELTZER2", "FUROREGT", "FUSILADE",
	"FUTO", "JESTER", "JESTER2", "KHAMELION", "KURUMA",
	"KURUMA2", "LYNX", "MASSACRO", "MASSACRO2", "NINEF",
	"NINEF2", "OMNIS", "PENUMBRA", "RAPIDGT", "RAPIDGT2",
	"RAPTOR", "SCHAFTER3", "SCHAFTER4", "SCHWARZE", "SEVEN70",
	"SULTAN", "SURANO", "SPECTER", "SPECTER2", "TAMPA2", "TROPOS", "VERLIERER2",
	"RUINER2", "PHANTOM2", "RUSTON", "COMET4", "COMET5", "NEON", "PARIAH", "RAIDEN", "REVOLTER", "SENTINEL3", "STREITER", "Z190",
	"GB200", "HOTRING", "ITALIGTO", "SCHLAGEN", "ZR380", "ZR3802", "ZR3803",
	"DRAFTER", "JUGULAR", "LOCUST", "NEO", "PARAGON", "PARAGON2"
};
static const char* SportsClassics1[] = {
	"BTYPE", "BTYPE2", "BTYPE3", "CASCO", "COQUETTE2",
	"FELTZER3", "JB700", "MAMBA", "MANANA", "MONROE",
	"PEYOTE", "PIGALLE", "STINGER", "STINGERGT", "TORNADO",
	"TORNADO2", "TORNADO3", "TORNADO4", "TORNADO5", "TORNADO6",
	"ZTYPE", "INFERNUS2", "TURISMO2", "RAPIDGT3", "RETINUE", "DELUXO", "GT500",
	"SAVESTRA", "STROMBERG", "VISERIS", "CHEBUREK", "FAGALOA", "JESTER3", "MICHELLI", "SWINGER",
	"DYNASTY", "PEYOTE2", "ZION3"
};
static const char* Super1[] = {
	"ADDER", "BANSHEE2", "BULLET", "CHEETAH", "ENTITYXF",
	"FMJ", "SHEAVA", "INFERNUS", "NERO", "NERO2","OSIRIS", "LE7B",
	"ITALIGTB", "ITALIGTB2", "PFISTER811", "PROTOTIPO", "REAPER", "SULTANRS", "T20",
	"TEMPESTA", "TURISMOR", "TYRUS", "VACCA", "VOLTIC", "ZENTORNO", "VOLTIC2", "PENETRATOR", "GP1", "XA21", "VIGILANTE",
	"CYCLONE", "VISIONE", "AUTARCH", "SC1", "ENTITY2", "TAIPAN", "TEZERACT", "TYRANT", "SCRAMJET", "DEVESTE",
	"EMERUS", "KRIEGER", "S80", "THRAX", "ZORRUSSO"
};
static const char* SUVs1[] = {
	"BALLER", "BALLER2", "BALLER3", "BALLER4", "BALLER5",
	"BALLER6", "BJXL", "CAVALCADE", "CAVALCADE2", "CONTENDER",
	"DUBSTA", "DUBSTA2", "FQ2", "GRANGER", "GRESLEY",
	"HABANERO", "HUNTLEY", "LANDSTALKER", "MESA", "MESA2",
	"PATRIOT", "RADI", "ROCOTO", "SEMINOLE", "SERRANO",
	"XLS", "XLS2", "PATRIOT2", "TOROS", "NOVAK"
};
static const char* Trailer1[] = {
	"ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2", "BALETRAILER", "BOATTRAILER",
	"DOCKTRAILER", "FREIGHTTRAILER", "GRAINTRAILER", "PROPTRAILER", "RAKETRAILER",
	"TANKER", "TANKER2", "TR2", "TR3", "TR4",
	"TRAILERLOGS", "TRAILERS", "TRAILERS2", "TRAILERS3", "TRAILERSMALL",
	"TRFLAT", "TVTRAILER"
};
static const char* Trains1[] = {
	"CABLECAR", "FREIGHT", "FREIGHTCAR", "FREIGHTCONT1", "FREIGHTCONT2",
	"FREIGHTGRAIN", "METROTRAIN", "TANKERCAR"
};
static const char* Utility1[] = {
	"AIRTUG", "CADDY", "CADDY2", "DOCKTUG", "FORKLIFT",
	"MOWER", "RIPLEY", "SADLER", "SADLER2", "SCRAP",
	"TOWTRUCK", "TOWTRUCK2", "TRACTOR", "TRACTOR2", "TRACTOR3",
	"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3"
};
static const char* Vans1[] = {
	"BISON", "BISON2", "BISON3", "BOBCATXL", "BOXVILLE",
	"BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BURRITO", "BURRITO2",
	"BURRITO3", "BURRITO4", "BURRITO5", "CAMPER", "GBURRITO",
	"GBURRITO2", "JOURNEY", "MINIVAN", "MINIVAN2", "PARADISE",
	"PONY", "PONY2", "RUMPO", "RUMPO2", "RUMPO3",
	"SPEEDO", "SPEEDO2", "SURFER", "SURFER2", "TACO",
	"YOUGA", "YOUGA2", "SPEEDO4"
};
static const char* Boats1[] = {
	"DINGHY", "DINGHY2", "DINGHY3", "DINGHY4", "JETMAX",
	"MARQUIS", "PREDATOR", "SEASHARK", "SEASHARK2", "SEASHARK3",
	"SPEEDER", "SPEEDER2", "SQUALO", "SUBMERSIBLE", "SUBMERSIBLE2",
	"SUNTRAP", "TORO", "TORO2", "TROPIC", "TROPIC2",
	"TUG"
};
static const char* Commercial1[] = {
	"BENSON", "BIFF", "HAULER", "MULE", "MULE2",
	"MULE3", "PACKER", "PHANTOM", "POUNDER", "STOCKADE",
	"STOCKADE3", "MULE4", "POUNDER2", "TERROBYTE", "CERBERUS2", "CERBERUSS3",
	"CERBERUS"
};
static const char* Compacts1[] = {
	"BLISTA", "BRIOSO", "DILETTANTE", "DILETTANTE2", "ISSI2",
	"PANTO", "PRAIRIE", "RHAPSODY", "ISSI3", "ISSI6", "ISSI4", "ISSI5",
	"ISSI7"
};
static const char* Coupes1[] = {
	"COGCABRIO", "EXEMPLAR", "F620", "FELON", "FELON2",
	"JACKAL", "ORACLE", "ORACLE2", "SENTINEL", "SENTINEL2",
	"WINDSOR", "WINDSOR2", "ZION", "ZION2"
};
static const char* Cycles1[] = {
	"BMX", "CRUISER", "FIXTER", "SCORCHER", "TRIBIKE",
	"TRIBIKE2", "TRIBIKE3"
};
static const char* Emergency1[] = {
	"AMBULANCE", "FBI", "FBI2", "FIRETRUK", "LGUARD",
	"PBUS", "PRANGER", "POLICE", "POLICE2", "POLICE3",
	"POLICE4", "POLICEB", "POLICEOLD1", "POLICEOLD2", "POLICET",
	"SHERIFF", "SHERIFF2", "RIOT", "RIOT2"
};
static const char* Helicopters1[] = {
	"ANNIHILATOR", "BLIMP", "BLIMP2", "BUZZARD", "BUZZARD2",
	"CARGOBOB", "CARGOBOB2", "CARGOBOB3", "CARGOBOB4", "FROGGER",
	"FROGGER2", "MAVERICK", "POLMAV", "SAVAGE", "SKYLIFT",
	"SUPERVOLITO", "SUPERVOLITO2", "SWIFT", "SWIFT2", "VALKYRIE",
	"VALKYRIE2", "VOLATUS", "HUNTER", "HAVOK", "AKULA", "SEASPARROW",
	"BLIMP3"
};
static const char* Industrial1[] = {
	"BULLDOZER", "CUTTER", "DUMP", "FLATBED", "GUARDIAN",
	"HANDLER", "MIXER", "MIXER2", "RUBBLE", "TIPTRUCK",
	"TIPTRUCK2"
};
static const char* Military1[] = {
	"BARRACKS", "BARRACKS2", "BARRACKS3", "CRUSADER", "RHINO", "BARRAGE", "CHERNOBOG", "KHANJALI", "THRUSTER", "SCARAB", "SCARAB2", "SCARAB3"
};
static const char* Motorcycles1[] = {
	"AKUMA", "AVARUS", "BAGGER", "BATI", "BATI2",
	"BF400", "CARBONRS", "CHIMERA", "CLIFFHANGER", "DAEMON",
	"DAEMON2", "DEFILER", "DOUBLE", "ENDURO", "ESSKEY",
	"FAGGIO", "FAGGIO2", "FAGGIO3", "GARGOYLE", "HAKUCHOU",
	"HAKUCHOU2", "HEXER", "INNOVATION", "LECTRO", "MANCHEZ",
	"NEMESIS", "NIGHTBLADE", "PCJ", "RATBIKE", "RUFFIAN",
	"SANCHEZ", "SANCHEZ2", "SANCTUS", "SHOTARO", "SOVEREIGN",
	"THRUST", "VADER", "VINDICATOR", "VORTEX", "WOLFSBANE",
	"ZOMBIEA", "ZOMBIEB", "DIABLOUS", "DIABLOUS2", "FCR",
	"FCR2", "OPPRESSOR", "AVENGER", "OPPRESSOR2", "DEATHBIKE2",
	"DEATHBIKE3", "DEATHBIKE", "RROCKET"
};
static const char* Muscle1[] = {
	"BLADE", "BUCCANEER", "BUCCANEER2", "CHINO", "CHINO2",
	"COQUETTE3", "DOMINATOR", "DOMINATOR2", "DUKES", "DUKES2",
	"GAUNTLET", "GAUNTLET2", "FACTION", "FACTION2", "FACTION3",
	"HOTKNIFE", "LURCHER", "MOONBEAM", "MOONBEAM2", "NIGHTSHADE",
	"PHOENIX", "PICADOR", "RATLOADER", "RATLOADER2", "RUINER", "RUINER2", "RUINER3",
	"SABREGT", "SABREGT2", "SLAMVAN", "SLAMVAN2", "SLAMVAN3",
	"STALION", "STALION2", "TAMPA", "VIGERO", "VIRGO",
	"VIRGO2", "VIRGO3", "VOODOO", "VOODOO2", "HERMES", "HUSTLER", "YOSEMITE", "DOMINATOR3", "ELLIE",
	"IMPALER3", "SLAMVAN6", "DOMINATOR6", "IMPALER4","SLAMVAN4","SLAMVAN5",
	"DOMINATOR4", "DOMINATOR5", "BRUISER", "BRUISER2", "BRUISER3", "RCBANDITO", "IMPALER2",
	"TULIP", "CLIQUE", "VAMOS", "IMPERATOR", "IMPERATOR2", "IMPERATOR3",
	"DEVIANT", "IMPALER", "GAUNTLET3", "GAUNTLET4"
};
static const char* OffRoad1[] = {
	"BFINJECTION", "BIFTA", "BLAZER", "BLAZER2", "BLAZER3",
	"BLAZER4", "BODHI2", "BRAWLER", "DLOADER", "DUBSTA3",
	"DUNE", "DUNE2", "INSURGENT", "INSURGENT2", "KALAHARI",
	"MARSHALL", "MESA3", "MONSTER", "RANCHERXL", "RANCHERXL2",
	"REBEL", "REBEL2", "SANDKING", "SANDKING2", "TECHNICAL",
	"TROPHYTRUCK", "TROPHYTRUCK2", "TECHNICAL2", "DUNE4", "DUNE5",
	"BLAZER5", "KAMACHO", "RIATA", "CARACARA", "FREECRAWLER", "MENACER",
	"MONSTER4", "MONSTER5", "BRUTUS", "BRUTUS2", "BRUTUS3", "MONSTER3",
	"CARACARA2", "HELLION"
};

static const char* CriminalEnterprise1[] = {
	"CONADA", "CORSITA", "GREENWOOD", "LM87", "TENF", "TORERO2", "VIGERO2", "SM722", "RHINEHART", "DRAUGUR", "POSTLUDE", "KANJOSJ", "OMNISEGT",
	"RUINER4",
};

static const char* DrugWars1[] = {
	"BOOR", "BRICKADE2", "BROADWAY", "CARGOPLANE2", "ENTITY3", "EUDORA", "EVERON2", "ISSI8", "JOURNEY2", "MANCHEZ3", "PANTHERE", "POWERSURGE", "R300", "SURFER3", "TAHOMA", "TULIP2", "PANTHERE",
};

extern std::string key;
extern std::string securekey_encode;
extern std::string external_ip_encode;

std::string getLast6Chars(const std::string& input) {
	return input.substr(input.length() - 6);
}

void TextWithOutline(const char* text, ImColor textColor, ImColor outlineColor = ImColor(0, 0, 0, 255), float outlineThickness = 1.0f) {
	ImVec2 pos = ImGui::GetCursorPos();
	for (float x = -outlineThickness; x <= outlineThickness; x += outlineThickness) {
		for (float y = -outlineThickness; y <= outlineThickness; y += outlineThickness) {
			if (x == 0 && y == 0) continue;
			ImGui::SetCursorPos(ImVec2(pos.x + x, pos.y + y));
			ImGui::TextColored(outlineColor, text);
		}
	}
	ImGui::SetCursorPos(pos);
	ImGui::TextColored(textColor, text);
}

void Gui::Overlay()
{
	g_overlayText.Render(); // Only Text Overlay

	static int		iFrames = 0;
	static clock_t	clockFrames = clock();
	static float	iFps;
	iFrames++;
	clock_t dif = clock() - clockFrames;

	if (dif > 1000)
	{
		iFps = iFrames / (dif / 500.f);
		iFrames = 0;
		clockFrames = clock();
	}

	std::string str = std::to_string(iFps);
	while (str.size() > str.find(".")) { str.pop_back(); }
	MessageString = str + " FPS";

	if (g_watermark == true)
	{
		ImGui::SetNextWindowBgAlpha(0.0f);
		if (ImGui::Begin(skCrypt("Redich Info"), NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs))
		{
			ImGui::SetWindowSize(skCrypt("Redich Info"), ImVec2(315, 89));
			ImGui::SetWindowPos(skCrypt("Redich Info"), ImVec2(11, 11));

			TextWithOutline(skCrypt("Redich.cc"), ImColor(255, 255, 0, 255));
			ImGui::SameLine(0.0f, 4.0f);

			char buffer[256];
			snprintf(buffer, sizeof(buffer), skCrypt("Exploit For GTAV %s | Frame Rate: %s |"), Version.c_str(), MessageString);
			TextWithOutline(buffer, ImColor(255, 255, 255, 255));
			ImGui::SameLine(0.0f, 4.0f);

			if (g_partner)
			{
				TextWithOutline(skCrypt("Special"), ImColor(0, 255, 0, 255));
				ImGui::SameLine(0.0f, 4.0f);
				TextWithOutline(skCrypt("Partner"), ImColor(0, 255, 0, 255));
				ImGui::SameLine(0.0f, 4.0f);

				snprintf(buffer, sizeof(buffer), skCrypt("Edition | Secure Key: %s"), securekey_encode);
				TextWithOutline(buffer, ImColor(255, 255, 255, 255));
			}
			else
			{
				TextWithOutline(skCrypt("Basic"), ImColor(255, 255, 255, 255));
				ImGui::SameLine(0.0f, 4.0f);

				snprintf(buffer, sizeof(buffer), skCrypt("Edition | Open Key: F4 | Secure Key: %s"), securekey_encode);
				TextWithOutline(buffer, ImColor(255, 255, 255, 255));
			}

			ImGui::End();
		}
	}
}

static const int numPlanes1 = sizeof(Planes1) / sizeof(Planes1[0]);
static const int numSedans1 = sizeof(Sedans1) / sizeof(Sedans1[0]);
static const int numService1 = sizeof(Service1) / sizeof(Service1[0]);
static const int numSports1 = sizeof(Sports1) / sizeof(Sports1[0]);
static const int numSportsClassics1 = sizeof(SportsClassics1) / sizeof(SportsClassics1[0]);
static const int numSuper1 = sizeof(Super1) / sizeof(Super1[0]);
static const int numSUVs1 = sizeof(SUVs1) / sizeof(SUVs1[0]);
static const int numTrailer1 = sizeof(Trailer1) / sizeof(Trailer1[0]);
static const int numTrains1 = sizeof(Trains1) / sizeof(Trains1[0]);
static const int numUtility1 = sizeof(Utility1) / sizeof(Utility1[0]);
static const int numVans1 = sizeof(Vans1) / sizeof(Vans1[0]);
static const int numBoats1 = sizeof(Boats1) / sizeof(Boats1[0]);
static const int numCommercial1 = sizeof(Commercial1) / sizeof(Commercial1[0]);
static const int numCompacts1 = sizeof(Compacts1) / sizeof(Compacts1[0]);
static const int numCoupes1 = sizeof(Coupes1) / sizeof(Coupes1[0]);
static const int numCycles1 = sizeof(Cycles1) / sizeof(Cycles1[0]);
static const int numEmergency1 = sizeof(Emergency1) / sizeof(Emergency1[0]);
static const int numHelicopters1 = sizeof(Helicopters1) / sizeof(Helicopters1[0]);
static const int numIndustrial1 = sizeof(Industrial1) / sizeof(Industrial1[0]);
static const int numMilitary1 = sizeof(Military1) / sizeof(Military1[0]);
static const int numMotorcycles1 = sizeof(Motorcycles1) / sizeof(Motorcycles1[0]);
static const int numMuscle1 = sizeof(Muscle1) / sizeof(Muscle1[0]);
static const int numOffRoad1 = sizeof(OffRoad1) / sizeof(OffRoad1[0]);
static const int numCriminalEnterprise1 = sizeof(CriminalEnterprise1) / sizeof(CriminalEnterprise1[0]);
static const int numDrugWars1 = sizeof(DrugWars1) / sizeof(DrugWars1[0]);
bool individualspanwer = true;

void ShowVehicleButtons(const char* categoryName, const char* items[], int itemCount) {
	ImGui::BulletText(categoryName); 
	ImGui::Text(""); ImGui::SameLine();
	for (int i = 0; i < itemCount; ++i) {

		if (i > 0 && i % 3 == 0)
			ImGui::NewLine();

		if (i > 0) 
			ImGui::SameLine();

		if (ImGui::Button(items[i])) {
			g_self.spawn_vehiclesss((char*)items[i]);
		}
	}
}

void Gui::Tick() {
	if (GetAsyncKeyState(key6) & Secure1)
	{
		ShowNotification(skCrypt("[F6 Hotkey] Join Invite Lobby"), 4.0f);
		change_session(InviteOnlySession);
	}

	if (GetAsyncKeyState(key5) & Secure1)
	{
		ShowNotification(skCrypt("[F5 Hotkey] Noclip"), 4.0f);
		g_self.noclip ^= true;
	}

    if (GetAsyncKeyState(key4) & Secure1)
        opened ^= true;

	if (GetAsyncKeyState(key3) & Secure1)
		partnerwindow ^= true;

	if (g_partner&& partnerwindow && opened)
	{
		if (ImGui::Begin(skCrypt("Partner Tool"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::SetWindowSize(skCrypt("Partner Tool"), ImVec2(415, 335));
			ImGui::Text(skCrypt("R3dich Partner Panel on V64 (Hotkey = F3)"), Version.c_str());
			ImGui::Separator();
			if (ImGui::CollapsingHeader(skCrypt("Vehicle Spawner")))
			{
				if (individualspanwer)
				{
					if (ImGui::Begin(skCrypt("Spawner"), NULL, ImGuiWindowFlags_NoCollapse))
					{
						ImGui::SetWindowSize(skCrypt("Spawner"), ImVec2(415, 935));

						ShowVehicleButtons(skCrypt("Sedans"), Sedans1, numSedans1);
						ShowVehicleButtons(skCrypt("Service"), Service1, numService1);
						ShowVehicleButtons(skCrypt("Sports"), Sports1, numSports1);
						ShowVehicleButtons(skCrypt("Sports Classics"), SportsClassics1, numSportsClassics1);
						ShowVehicleButtons(skCrypt("Super"), Super1, numSuper1);
						ShowVehicleButtons(skCrypt("SUVs"), SUVs1, numSUVs1);
						ShowVehicleButtons(skCrypt("Trailer"), Trailer1, numTrailer1);
						ShowVehicleButtons(skCrypt("Trains"), Trains1, numTrains1);
						ShowVehicleButtons(skCrypt("Utility"), Utility1, numUtility1);
						ShowVehicleButtons(skCrypt("Vans"), Vans1, numVans1);
						ShowVehicleButtons(skCrypt("Boats"), Boats1, numBoats1);
						ShowVehicleButtons(skCrypt("Commercial"), Commercial1, numCommercial1);
						ShowVehicleButtons(skCrypt("Compacts"), Compacts1, numCompacts1);
						ShowVehicleButtons(skCrypt("Coupes"), Coupes1, numCoupes1);
						ShowVehicleButtons(skCrypt("Cycles"), Cycles1, numCycles1);
						ShowVehicleButtons(skCrypt("Emergency"), Emergency1, numEmergency1);
						ShowVehicleButtons(skCrypt("Planes"), Planes1, numPlanes1);
						ShowVehicleButtons(skCrypt("Helicopters"), Helicopters1, numHelicopters1);
						ShowVehicleButtons(skCrypt("Industrial"), Industrial1, numIndustrial1);
						ShowVehicleButtons(skCrypt("Military"), Military1, numMilitary1);
						ShowVehicleButtons(skCrypt("Motorcycles"), Motorcycles1, numMotorcycles1);
						ShowVehicleButtons(skCrypt("Muscle"), Muscle1, numMuscle1);
						ShowVehicleButtons(skCrypt("Off-Road"), OffRoad1, numOffRoad1);
						ShowVehicleButtons(skCrypt("Criminal Enterprise"), CriminalEnterprise1, numCriminalEnterprise1);
						ShowVehicleButtons(skCrypt("Drug Wars"), DrugWars1, numDrugWars1);
					}
					ImGui::End();
				}
				else
				{
					ShowVehicleButtons(skCrypt("Planes"), Planes1, numPlanes1);
					ShowVehicleButtons(skCrypt("Sedans"), Sedans1, numSedans1);
					ShowVehicleButtons(skCrypt("Service"), Service1, numService1);
					ShowVehicleButtons(skCrypt("Sports"), Sports1, numSports1);
					ShowVehicleButtons(skCrypt("Sports Classics"), SportsClassics1, numSportsClassics1);
					ShowVehicleButtons(skCrypt("Super"), Super1, numSuper1);
					ShowVehicleButtons(skCrypt("SUVs"), SUVs1, numSUVs1);
					ShowVehicleButtons(skCrypt("Trailer"), Trailer1, numTrailer1);
					ShowVehicleButtons(skCrypt("Trains"), Trains1, numTrains1);
					ShowVehicleButtons(skCrypt("Utility"), Utility1, numUtility1);
					ShowVehicleButtons(skCrypt("Vans"), Vans1, numVans1);
					ShowVehicleButtons(skCrypt("Boats"), Boats1, numBoats1);
					ShowVehicleButtons(skCrypt("Commercial"), Commercial1, numCommercial1);
					ShowVehicleButtons(skCrypt("Compacts"), Compacts1, numCompacts1);
					ShowVehicleButtons(skCrypt("Coupes"), Coupes1, numCoupes1);
					ShowVehicleButtons(skCrypt("Cycles"), Cycles1, numCycles1);
					ShowVehicleButtons(skCrypt("Emergency"), Emergency1, numEmergency1);
					ShowVehicleButtons(skCrypt("Helicopters"), Helicopters1, numHelicopters1);
					ShowVehicleButtons(skCrypt("Industrial"), Industrial1, numIndustrial1);
					ShowVehicleButtons(skCrypt("Military"), Military1, numMilitary1);
					ShowVehicleButtons(skCrypt("Motorcycles"), Motorcycles1, numMotorcycles1);
					ShowVehicleButtons(skCrypt("Muscle"), Muscle1, numMuscle1);
					ShowVehicleButtons(skCrypt("Off-Road"), OffRoad1, numOffRoad1);
					ShowVehicleButtons(skCrypt("Criminal Enterprise"), CriminalEnterprise1, numCriminalEnterprise1);
					ShowVehicleButtons(skCrypt("Drug Wars"), DrugWars1, numDrugWars1);
				}
			}

			if (ImGui::CollapsingHeader(skCrypt("Spawner Settings")))
			{
				ImGui::Checkbox(skCrypt("Spawn with Ride"), &g_self.spawn_in_vehicle);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Spawn with Engine on"), &g_self.engine_on);

				ImGui::Checkbox(skCrypt("Spawn with Godmode"), &g_self.spawn_god);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("individual Spawner Window"), &individualspanwer);
			}

			if (ImGui::CollapsingHeader(skCrypt("Vehicles")))
			{
				if (ImGui::Button(skCrypt("Repair (!)")))
				{
					Queue::Instance()->Add([] {
						Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
						{
							VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0);
							VEHICLE::SET_VEHICLE_FIXED(Vehicle);
							VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
						}
					});
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Max Upgrade")))
				{
					Queue::Instance()->Add([] {
						Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
						{
							VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
							for (int i = 0; i < 50; i++)
							{
								VEHICLE::SET_VEHICLE_MOD(Vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, i) - 1, false);
							}
							VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle, 7, 84, 61);
							VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle, 7, 84, 61);
							VEHICLE::SET_VEHICLE_FIXED(Vehicle);
							VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
							VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0);
						}
					});
				}

				if (ImGui::Button(skCrypt("Open All Doors")))
				{
					Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
					{
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 0, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 1, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 2, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 3, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 4, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 5, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 6, true, false);
						VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 7, true, false);
					}
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Close All Doors")))
				{
					Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
					{
						VEHICLE::SET_VEHICLE_DOORS_SHUT(Vehicle, false);
					}
				}
			}

			if (ImGui::CollapsingHeader(skCrypt("Time Selector")))
			{
				static int hours{ 1 }, min{ 1 }, sec{ 1 };
				ImGui::InputInt(skCrypt("hour"), &hours, 0, 0);
				ImGui::InputInt(skCrypt("min"), &min, 0, 0);
				ImGui::InputInt(skCrypt("sec "), &sec, 0, 0);

				if (ImGui::Button(skCrypt("Apply")))
				{
					NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(hours, min, sec);
				}
			}

			if (ImGui::CollapsingHeader(skCrypt("Stelath Loop Custom Delay")))
			{
				ImGui::BulletText(skCrypt("Stelath Delay"));
				ImGui::SliderInt(skCrypt("Stelath#1 Delay"), &g_money.customdelay700, 1, 5000, "%d%ms");
				ImGui::SliderInt(skCrypt("Stelath#2 Delay"), &g_money.customdelay1000, 1, 5000, "%d%ms");
			}

			if (ImGui::CollapsingHeader(skCrypt("Private Sealth")))
			{
				ImGui::BulletText(skCrypt("Private Stealth"));

				if (ImGui::Button(skCrypt("Bunker Stealth"))) {
					ShowNotification(skCrypt("This action takes 6s"), 7.0f);
					Queue::Instance()->Add([] {
						g_money.basket_transaction(-1799524201, -1018905335, 4, {
							{-1026265157, -1601574614, 0, Secure1, Secure1}
							});
						Script::Current()->ScriptYield(std::chrono::milliseconds(1500));

						g_money.basket_transaction(-1799524201, 2108234284, 4, {
							{-1026265157, -1601574614, 0, Secure1, Secure1},
							{-1121615002, -1, 0, 0, 40},
							{297710065, -1121615002, 0, 40, Secure1}
							});
						Script::Current()->ScriptYield(std::chrono::milliseconds(1500));

						g_money.basket_transaction(69656641, -886368739, 4, {
							{-1121615002, -1, 0, Secure1, -40},
							{-1573825732, -1, 0, Secure1, Secure1}
							});
						Script::Current()->ScriptYield(std::chrono::milliseconds(1500));

						g_money.basket_transaction(-1799524201, -1018905335, 4, {
							{-1026265157, -1826424790, 0, Secure1, Secure1}
							});
						Script::Current()->ScriptYield(std::chrono::milliseconds(1500));

						g_money.basket_transaction(-1799524201, -788753717, 4, {
							{-1026265157, -1826424790, 0, Secure1, Secure1},
							{-1573825732, -1, 0, Secure1, Secure1},
							{-1818214114, -1573825732, 2500000, 0, Secure1}
							});
					});
				}

				if (ImGui::Button(skCrypt("Spcial Cargo Stealth")))
				{
					ShowNotification(skCrypt("Use in Sell Mission"), 7.0f);
					Queue::Instance()->Add([] {
						g_money.basket_transaction(-1799524201, -788753717, g_money.walletorbank, {
							{ -937301143, 1679637634, 0, Secure1, Secure1 },
							{ 1295461001, -1, 0, Secure1, Secure1 },
							{ 1783677420, 1295461001, 6000000, 0, Secure1 }
							});
					});
				}
			}
		}
		ImGui::End();
	}

	if (opened) {
		if (ImGui::Begin(skCrypt("Redich"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
		{

			ImGui::SetWindowSize(skCrypt("Redich"), ImVec2(515, 435));
			ImGui::Text(skCrypt("Redich.cc"));
			ImGui::Separator();

			if (ImGui::CollapsingHeader(skCrypt("Local")))
			{
				ImGui::BulletText(skCrypt("Self-Local")); //Self-Weapon

				ImGui::Checkbox(skCrypt("Godmode"), &g_self.godmode);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Never Wanted"), &g_self.neverwanted);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("No Ragdoll"), &g_self.noragdoll);

				ImGui::Checkbox(skCrypt("Invisible"), &g_self.invisible);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("No idle Kick"), &g_self.noidlekick);

				ImGui::Checkbox(skCrypt("Money Rain"), &g_self.moneydrop);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(140.f);
				ImGui::SliderFloat(skCrypt("Range"), &g_self.moneyrainrange, 1.f, 10.f, "%.3f", 0);

				if (ImGui::Button(skCrypt("Get Health"))) { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 400, 1); }
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Get Armour"))) { PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200); }
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Skip Cutscene"))) { CUTSCENE::STOP_CUTSCENE(true); CUTSCENE::STOP_CUTSCENE_IMMEDIATELY(); }
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Clear ped"))) { PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID()); }
				if (ImGui::Button(skCrypt("Dry Ped"))) { PED::CLEAR_PED_WETNESS(PLAYER::PLAYER_PED_ID()); }
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Suicide")))
				{
					ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, 0);
				}
				ImGui::BulletText(skCrypt("Self-Movement"));

				ImGui::Checkbox(skCrypt("Noclip"), &g_self.noclip);

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Teleport to Waypoint")))
				{
					g_self.teleport_to_markerr();
				}

				ImGui::BulletText(skCrypt("Self-Weapon")); //Self-Weapon

				ImGui::Checkbox(skCrypt("Infinity Ammo"), &g_self.infinityammo); //Infinity-Ammo
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Rapid Fire"), &g_self.rapidfire);
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Get All Weapons"))) //Get All Weapons
				{
					Queue::Instance()->Add([] {

						const int Weapons[] = { 0x181410A1, 0xC78D71B4, 0x45CD9CF3, 0xDB26713A, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, 0x3813FC08,
							0xA2719263, 0x8BB05FD7, 0xF9DCBF2D, 0xD8DF3C3C, 0xDD5DF8D9, 0xDFE37640, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, 0xBFE256D4, 0x88374054, 0x83839C4, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0x2BE6766B, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x555AF99A, 0xEF951FBB, 0x12E82D3D, 0x394F415C, 0xFAD1F1C9, 0x969C3D67, 0x84D6FAFD, 0x624FE830, 0xDBBD7280, 0xA914799, 0x6A6C02E0, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xA0973D5E, 0xAB564B93, 0xBA45E8B8, 0xFBAB5776, 0x060EC506, 0xAF3696A1, 0xD7DBF707, 0x476BF155, 0xB62D1F67,0xF38A0747, 0x167C5572, 0x2C9CA024, 0xA991DAE8, 0xD6678401, 0x6AA85572
						};

						for (int i = 0; i < (sizeof(Weapons) / 4); i++)
						{
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
							Script::Current()->ScriptYield(std::chrono::milliseconds(10));
						}
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Remove All Weapons"))) //Remove All Weapons
				{
					WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), true);
				}
			}
			if (ImGui::CollapsingHeader(skCrypt("Money")))
			{
				ImGui::BulletText(skCrypt("Setting Transactions"));
				ImGui::Checkbox(skCrypt("Randomize Stealth Value"), &g_money.g_moresafe);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(skCrypt("Change the value of stealth loop coming in every tick, making it harder for R* to detect it"));
				}

				if (ImGui::Button(skCrypt("Receive in Wallet")))
				{
					ShowNotification(skCrypt("Now receive stealth money in wallet"), 7.0f);
					g_money.walletorbank = 1;
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Receive in Bank")))
				{
					ShowNotification(skCrypt("Now receive stealth money in bank"), 7.0f);
					g_money.walletorbank = 4;
				}

				ImGui::BulletText(skCrypt("Limited Transactions"));

				if(ImGui::Button(skCrypt("Seed Money#1")))
				{
					Queue::Instance()->Add([] {
						g_money.basket_transaction(1474183246, 1445302971, 4,
							{
								 {(int)2708796979, -1, 15000000, 0, Secure1}
							});
					});
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Seed Money#2")))
				{
					Queue::Instance()->Add([] {
						g_money.basket_transaction(1474183246, 1445302971, 4,
							{
								 {(int)3986141121, -1, 7000000, 0, Secure1}
							});
					});
				}

				ImGui::BulletText(skCrypt("Stealth Loop Transactions"));

				ImGui::Checkbox(skCrypt("Stealth Loop#1"), &g_money.betloop);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Stealth Loop#2"), &g_money.ncloop);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(skCrypt("Need a Night Club"));
				}

				ImGui::BulletText(skCrypt("General Transactions"));

				if (ImGui::Button(skCrypt("Re-Initialize Stealth#2")))
				{
					Queue::Instance()->Add([] {
						//ShowNotification(skCrypt("Setting up Transaction... Please wait for seconds"), 7.0f);
						ShowNotification(skCrypt("Setting up Transaction... Please wait for seconds"), 7.0f);
						g_money.basket_transaction(-1799524201, -1018905335, 8, { {545376149, 129314700, 0, 1, 1} });

						Script::Current()->ScriptYield(std::chrono::milliseconds(2000));

						g_money.basket_transaction(-1799524201, 2108234284, 8, {
								{545376149, 129314700, 0, Secure1, Secure1},
								{-544423649, -1, 0, 0, 50000},
								{1435847077, -544423649, 0, 50000, Secure1}
							});

					});
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Remove Moeny")))
				{
					Queue::Instance()->Add([] {
						g_money.basket_transaction(RAGE_JOAAT("CATEGORY_SERVICE_WITH_THRESHOLD"), RAGE_JOAAT("NET_SHOP_ACTION_SPEND"), 4,
							{
								 {RAGE_JOAAT("SERVICE_SPEND_JUGALLO_BOSS_VEHICLE_REQUEST"), -1, 10000000, 0, Secure1}
							});
					});
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Casino Chips")))
				{
					Queue::Instance()->Add([] {
						g_money.basket_transaction(1950528552, -126744038, g_money.walletorbank, {
							{ 657241867, -1, 0, 0, 2500000},
							{1196301501, -1, 0, 0, Secure1},
							});

						Script::Current()->ScriptYield(std::chrono::milliseconds(1000));

						*script_global(1964419).as<int*>() = Secure1;
					});
				}

				ImGui::BulletText(skCrypt("Exchange Transactions"));

				if (ImGui::Button(skCrypt("Deposit Balance")))
				{
					int iVar0;
					int player_balance_wallet = MONEY::NETWORK_GET_VC_WALLET_BALANCE(-1);
					STATS::STAT_GET_INT(RAGE_JOAAT("mpply_last_mp_char"), &iVar0, -1);
					NETSHOPPING::NET_GAMESERVER_TRANSFER_WALLET_TO_BANK(iVar0, player_balance_wallet);
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Withdraw Balance")))
				{
					int iVar0;
					int player_balance_wallet = MONEY::NETWORK_GET_VC_BANK_BALANCE();
					STATS::STAT_GET_INT(RAGE_JOAAT("mpply_last_mp_char"), &iVar0, -1);
					NETSHOPPING::NET_GAMESERVER_TRANSFER_BANK_TO_WALLET(iVar0, player_balance_wallet);
				}

				ImGui::BulletText(skCrypt("Outdated Transactions"));

				if (ImGui::Button(skCrypt("New-Gen Orbital")))
				{
					Queue::Instance()->Add([] {
						g_money.basket_transaction(RAGE_JOAAT("CATEGORY_SERVICE_WITH_THRESHOLD"), RAGE_JOAAT("NET_SHOP_ACTION_EARN"), g_money.walletorbank,
							{
								 {(int)2896648878, -1, 500000, 0, Secure1}
							});
					});
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(skCrypt("Use At Own RISK"));
				}
				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Orbital Refund-1")))
				{
					Queue::Instance()->Add([] {
						*script_global(1962237).as<int*>() = Secure1;
						Script::Current()->ScriptYield(std::chrono::milliseconds(500));
						*script_global(1962237).as<int*>() = 0;
					});
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Orbital Refund-2")))
				{
					Queue::Instance()->Add([] {
						*script_global(1962237).as<int*>() = Secure1 + 1;
						Script::Current()->ScriptYield(std::chrono::milliseconds(500));
						*script_global(1962237).as<int*>() = 0;
					});
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Casino Chips 5k")))
				{
					Queue::Instance()->Add([] {
						*script_global(1964419).as<int*>() = Secure1;
					});
				}
			}
			if (ImGui::CollapsingHeader(skCrypt("Rank")))
			{
				ImGui::BulletText(skCrypt("Change Rank"));

				ImGui::InputInt(skCrypt("Input: "), &rank_value, 0, 0);//Rank Input

				ImGui::RadioButton(skCrypt("Admin Gift"), &rank_mod, 1);

				ImGui::SameLine();

				ImGui::RadioButton(skCrypt("Instant"), &rank_mod, 2);

				if (ImGui::Button(skCrypt("Change Rank")))
				{
					if (rank_mod == 1)
					{
						if (rank_value <= 1)
						{
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_SET_RP_GIFT_ADMIN"), 1, TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_SET_RP_GIFT_ADMIN"), 1, TRUE);
							STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), 1);
							ShowNotification(skCrypt("Please change session to apply"), 7.0f);
						}
						if (rank_value >= 2)
						{
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_SET_RP_GIFT_ADMIN"), rp_value(rank_value), TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_SET_RP_GIFT_ADMIN"), rp_value(rank_value), TRUE);
							ShowNotification(skCrypt("Please change session to apply"), 7.0f);
						}
					}
					if (rank_mod == 2)
					{
						if (rank_value <= 1)
						{
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_XP_FM"), 1, TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_XP_FM"), 1, TRUE);
							STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), 1);
						}
						if (rank_value >= 2)
						{
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_XP_FM"), rp_value(rank_value), TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_XP_FM"), rp_value(rank_value), TRUE);
							STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(rank_value));
						}
					}
					if (rank_value == 3)
					{
						STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_RANK_FM_SA"), rank_value, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_RANK_FM_SA"), rank_value, TRUE);
						STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(rank_value));
					}
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Change Crew Rank")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_0"), rp_value(rank_value), TRUE);
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_1"), rp_value(rank_value), TRUE);
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_2"), rp_value(rank_value), TRUE);
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_3"), rp_value(rank_value), TRUE);
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_4"), rp_value(rank_value), TRUE);
				}
			}
			if (ImGui::CollapsingHeader(skCrypt("Unlock")))
			{
				ImGui::BulletText(skCrypt("Unlock"));

				if (ImGui::Button(skCrypt("Unlock All")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_all();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Unlock Achievement")))
				{
					g_unlock.unlock_achieve();
					
				}
				ImGui::SameLine();
				ImGui::BeginDisabled();
				if (ImGui::Button(skCrypt("Unlock Vehicles For Gta+")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_removed_vehicle();
					});
				}
				ImGui::EndDisabled();
				ImGui::Separator();

				if (ImGui::Button(skCrypt("Some Stuff")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_stuff();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Bunker Research")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_bunker();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Clothes")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_clothes();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Weapon")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_weapon();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Vehicle")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_vehicle();
					});
				}

				if (ImGui::Button(skCrypt("Wheels")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_rims();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Parachutes")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_para();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Tattoos")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_tattoos();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Hairstyle")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_hair();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Armor")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_armor();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Snack")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_snack();
					});
				}

				if (ImGui::Button(skCrypt("Statue")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_statue();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Office Money")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_office_money();
						ShowNotification(skCrypt("Sell special cargo for application"), 7.0f);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("FastRun")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_fastrun();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Gender change")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_gender();
						ShowNotification(skCrypt("Please Change the Session"), 7.0f);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Ability")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_stat();
					});
				}

				if (ImGui::Button(skCrypt("LSC Parts")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_lsc();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Chrome Wheels")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_chrims();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Clear Badsport")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_badsport();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Xmas 2023")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_xmas();
					});
				}

				if (ImGui::Button(skCrypt("Free Appearance Change")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_freeapp();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Edit Gun Van")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_gunvan();
					});
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Haloween DLC")))
				{
					Queue::Instance()->Add([] {
						g_unlock.unlock_halodlc();
					});
				}
			}

			if (ImGui::CollapsingHeader(skCrypt("Heist")))
			{
				ImGui::BulletText(skCrypt("Diamond Casino - Change Approach"));
				if (ImGui::Button(skCrypt("Silent")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_APPROACH"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_APPROACH"), 1, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Big Con")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_APPROACH"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_APPROACH"), 2, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Aggressive")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_APPROACH"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_APPROACH"), 3, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}

				ImGui::BulletText(skCrypt("Diamond Casino - Change Target"));

				if (ImGui::Button(skCrypt("Diamond")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 3, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Gold")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 1, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Art")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 2, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Cash")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 0, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}

				ImGui::BulletText(skCrypt("Diamond Casino - Setup"));

				if (ImGui::Button(skCrypt("Silent Setup")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_COMPLETEDPOSIX"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_WEAPS"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CAS_HEIST_FLOW"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_POI"), 1023, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_ACCESSPOINTS"), 2047, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET1"), 127, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET0"), 262270, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_COMPLETEDPOSIX"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_WEAPS"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CAS_HEIST_FLOW"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_POI"), 1023, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_ACCESSPOINTS"), 2047, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET1"), 127, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET0"), 262270, true);
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Big con Setup")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_COMPLETEDPOSIX"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_WEAPS"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CAS_HEIST_FLOW"), -1, true);
					//STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_POI"), 1023, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_ACCESSPOINTS"), 2047, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET1"), 159, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET0"), 524118, true);

					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_COMPLETEDPOSIX"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_WEAPS"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CAS_HEIST_FLOW"), -1, true);
					//STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_POI"), 1023, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_ACCESSPOINTS"), 2047, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET1"), 159, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET0"), 524118, true);
				}

				ImGui::SameLine();

				if (ImGui::Button(skCrypt("Aggressive Setup")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_COMPLETEDPOSIX"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_WEAPS"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CAS_HEIST_FLOW"), -1, true);
					//STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_POI"), 1023, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_ACCESSPOINTS"), 2047, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET1"), 799, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET0"), 3670102, true);

					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_COMPLETEDPOSIX"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_WEAPS"), 1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CAS_HEIST_FLOW"), -1, true);
					//STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_POI"), 1023, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_ACCESSPOINTS"), 2047, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 4, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 3, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), -1, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 2, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET1"), 799, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET0"), 3670102, true);
				}

				ImGui::BulletText(skCrypt("Diamond Casino - Difficulty"));
				if (ImGui::Button(skCrypt("Normal")))
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), 0, true);
					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), 0, true);
					ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Hard")))
				{
					Queue::Instance()->Add([] {
						for (int i = 1; i <= 3; i++)
						{
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), i, true);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), i, true);
						}
						ShowNotification(skCrypt("Please re - enter the arcade"), 7.0f);
					});
				}

				ImGui::BulletText(skCrypt("Diamond Casino - Player`s Cut"));

				ImGui::BeginDisabled();
				ImGui::SliderInt(skCrypt("#0"), &g_money.publicint1, 0, 160, "%d%%");
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Your Cut(Non-host)")))
				{
					*script_global(2685444 + 6641).as<int*>() = g_money.publicint1;
				}

				ImGui::SliderInt(skCrypt("#1"), &g_money.publicint2, 0, 160, "%d%%");
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Your Cut(Host)")))
				{
					*script_global(1971648 + 831 + 56 + 1).as<int*>() = g_money.publicint2;
					ShowNotification(skCrypt("Only works when you are the host of the heist!"), 7.0f);
				}
				ImGui::SliderInt(skCrypt("#2"), &g_money.publicint3, 0, 160, "%d%%");
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Player#2 Cut")))
				{
					*script_global(1971648 + 831 + 56 + 2).as<int*>() = g_money.publicint2;
					ShowNotification(skCrypt("Only works when you are the host of the heist!"), 7.0f);
				}
				ImGui::SliderInt(skCrypt("#3"), &g_money.publicint4, 0, 160, "%d%%");
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Player#3 Cut")))
				{
					*script_global(1971648 + 831 + 56 + 3).as<int*>() = g_money.publicint2;
					ShowNotification(skCrypt("Only works when you are the host of the heist!"), 7.0f);
				}
				ImGui::SliderInt(skCrypt("#4"), &g_money.publicint5, 0, 160, "%d%%");
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Player#4 Cut")))
				{
					*script_global(1971648 + 831 + 56 + 3).as<int*>() = g_money.publicint2;
					ShowNotification(skCrypt("Only works when you are the host of the heist!"), 7.0f);
				}
				ImGui::EndDisabled();
			}

			if (ImGui::CollapsingHeader(skCrypt("Stats")))
			{
				ImGui::BulletText("Stats - Compete"); ImGui::InputInt("1", &rcwon, 0, 0); ImGui::SameLine(); if (ImGui::Button("Race won")) { STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_WON"), rcwon, true); }ImGui::InputInt("2", &rclost, 0, 0); ImGui::SameLine(); if (ImGui::Button("Race lost")) { STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_LOST"), rclost, true); }ImGui::InputInt("3", &rcpt, 0, 0); ImGui::SameLine(); if (ImGui::Button("Race Record")) { int rcpt_hour = rcpt * 3600000; STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_RACES"), rcpt_hour, true); } if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Unit: Hour\nIf you enter 3 in the above input and press the Apply button, the total play time will be adjusted to 3 hours."); }ImGui::InputInt("4", &dmwon, 0, 0); ImGui::SameLine(); if (ImGui::Button("Death Match Won")) { STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_WON"), dmwon, true); } ImGui::InputInt("5", &dmlost, 0, 0); ImGui::SameLine(); if (ImGui::Button("Death Match Lost")) { STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_LOST"), dmlost, true); }ImGui::InputInt("6", &dmpt, 0, 0); ImGui::SameLine(); if (ImGui::Button("Death Match Record")) { int dmpt_hour = dmpt * 3600000; STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_DEATHMAT"), dmpt_hour, true); }if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Unit: Hour\nIf you enter 3 in the above input and press the Apply button, the total play time will be adjusted to 3 hours."); }
				ImGui::BulletText("Stats - Cash earned"); ImGui::InputInt("7", &jobs, 0, 0); ImGui::SameLine(); if (ImGui::Button("Jobs")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_JOBS"), jobs, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_JOBS"), jobs, true); }ImGui::InputInt("8", &betting, 0, 0); ImGui::SameLine(); if (ImGui::Button("Betting")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_BETTING"), betting, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_BETTING"), betting, true); }ImGui::InputInt("9", &shared, 0, 0); ImGui::SameLine(); if (ImGui::Button("Share")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_BETTING"), shared, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_BETTING"), shared, true); }ImGui::InputInt("10", &selling_veh, 0, 0); ImGui::SameLine(); if (ImGui::Button("Selling Vehicles")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_SELLING_VEH"), selling_veh, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_SELLING_VEH"), selling_veh, true); }ImGui::InputInt("11", &goodsp, 0, 0); ImGui::SameLine(); if (ImGui::Button("Misc")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_GOOD_SPORT"), goodsp, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_GOOD_SPORT"), goodsp, true); }ImGui::InputInt("12", &pickedup, 0, 0); ImGui::SameLine(); if (ImGui::Button("Picked up")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_PICKED_UP"), pickedup, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_PICKED_UP"), pickedup, true); }
				ImGui::BulletText("Stats - Income and expenses"); ImGui::InputInt("13", &cashtotale, 0, 0); ImGui::SameLine(); if (ImGui::Button("Total Income")) { STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_EVC"), cashtotale, true); } ImGui::InputInt("14", &cashtotals, 0, 0); ImGui::SameLine(); if (ImGui::Button("Total expenses")) { STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_SVC"), cashtotals, true); }
				ImGui::BulletText("Stats - Play time"); ImGui::InputInt("15", &totalplay, 0, 0); ImGui::SameLine(); if (ImGui::Button("Slot 1")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTAL_PLAYING_TIME"), totalplay * 3600000, true); } if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Unit: Hour\nIf you enter 3 in the above input and press the Apply button, the total play time will be adjusted to 3 hours."); } ImGui::InputInt("15-1", &totalplay, 0, 0); ImGui::SameLine(); if (ImGui::Button("Slot 2")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTAL_PLAYING_TIME"), totalplay * 3600000, true); } if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Unit: Hour\nIf you enter 3 in the above input and press the Apply button, the total play time will be adjusted to 3 hours."); } ImGui::InputInt("16", &mpplay, 0, 0); ImGui::SameLine(); if (ImGui::Button("Online")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME"), mpplay * 3600000, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), mpplay * 3600000, true); } if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Unit: Hour\nIf you enter 3 in the above input and press the Apply button, the total play time will be adjusted to 3 hours."); } 
				ImGui::BulletText("Stats - Misc"); ImGui::InputInt("17", &removedmoney, 0, 0); ImGui::SameLine(); if (ImGui::Button("Removed Money Stats")) { STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_CONTACT_SERVICE"), removedmoney, true); STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_CONTACT_SERVICE"), removedmoney, true); } ImGui::InputInt("18", &achieve, 0, 0); ImGui::SameLine(); if (ImGui::Button("Custom Achievements")) { if (achieve >= 77) {} else if (achieve >= 1) { PLAYER::GIVE_ACHIEVEMENT_TO_PLAYER(achieve); } else {} }
			}

			if (ImGui::CollapsingHeader(skCrypt("Misc")))
			{
				ImGui::BulletText(skCrypt("Misc"));

				ImGui::Checkbox(skCrypt("Bypass Transaction Error"), &g_misc.bypasserror);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("No Idle Camera"), &g_misc.noidlecam);

				ImGui::Checkbox(skCrypt("Always Show Balance"), &g_misc.showbalance);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Remove BusySpin"), &g_misc.busyspin);

				ImGui::Checkbox(skCrypt("Box ESP"), &g_misc.esp_box);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Line ESP"), &g_misc.esp_line);


				if (ImGui::Button(skCrypt("Skip Prologue")))
				{
					STATS::SET_PROFILE_SETTING_PROLOGUE_COMPLETE();
					ShowCenterMessage(skCrypt("Join Lobby right away!"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Force Cloud Save")))
				{
					STATS::STAT_SAVE(0, 0, 3, 0);
					ShowCenterMessage(skCrypt("Trying to Save... Please wait a moment"), 7.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Session Eject")))
				{
					NETWORK::NETWORK_BAIL(0, 0, 0);
					ShowNotification(skCrypt(":)"), 7.0f);
				}

				if (ImGui::Button(skCrypt("Turn on Snow Fall")))
				{
					*script_global(262145 + 4413).as<int*>() = 1;
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Turn off Snow Fall")))
				{
					*script_global(262145 + 4413).as<int*>() = 0;
				}

				if (ImGui::Button(skCrypt("Join New Invite only Lobby")))
				{
					change_session(InviteOnlySession);
				}

			}

			if (ImGui::CollapsingHeader(skCrypt("Setting")))
			{
				ImGui::BulletText(skCrypt("Setting"));

				if (ImGui::Button(skCrypt("Turn on Watermark")))
				{
					g_watermark = true;
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Turn off Watermark")))
				{
					g_watermark = false;
				}

				if (ImGui::Button(skCrypt("Re-Build Discord RPC")))
				{
					ShowCenterMessage(skCrypt("Ininitializing Discord RPC... This take a few seconds"), 7.0f);
					g_feat.DiscordInit();
				}
				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Destroy Discord RPC")))
				{
					ShowCenterMessage(skCrypt("Discord RPC Destroyed"), 7.0f);
					g_feat.DiscordDestroy();
				}

				ImGui::Separator();

				if (ImGui::Button(skCrypt("Unload")))
				{
					running = false;
				}
			}

			
			if (ImGui::CollapsingHeader(skCrypt("Member List")))
			{
				ImGui::BulletText(skCrypt("R3dich Devs"));
				ImGui::Text(skCrypt("Volume: R3dich Founder & Client Dev"));
				ImGui::Text(skCrypt("Lynnseo: R3dich Founder & Util Dev"));

				ImGui::BulletText(skCrypt("Redich Group Member"));
				ImGui::Text(skCrypt("Lanier: Some Api & Helper Bot Dev"));
				ImGui::Text(skCrypt("Icedoomfist: Scripts & Heist Function Provider"));

				ImGui::BulletText(skCrypt("Others"));
				ImGui::Text(skCrypt("Pocakking: BigbaseV2"));
				ImGui::Text(skCrypt("UnknownCheats: Some Code Provider"));
				ImGui::Text(skCrypt("Swasd: Security advisory"));
			}
			
			
			if (g_partner)
			{
				/*
				if (ImGui::CollapsingHeader("Partner"))
				{
					/*
					ImGui::BulletText("ÆäÀÌÅ© ¸Ó´Ï");
					ImGui::SliderInt("ÆäÀÌÅ© ¸Ó´Ï ¾×¼ö 1", &g_gui.fakeamount, 1, INT_MAX, "%d%$");
					ImGui::InputInt("ÆäÀÌÅ© ¸Ó´Ï ¾×¼ö 2", &g_gui.fakeamount, 0, 0);
					if (ImGui::Button("ÆäÀÌÅ© ¸Ó´Ï ¾ò±â"))
					{
						HUD::CHANGE_FAKE_MP_CASH(g_gui.fakeamount, g_gui.fakeamount);
					}
					ImGui::SameLine();
					if (ImGui::Button("ÆäÀÌÅ© ¸Ó´Ï ¾ò±â (¿ÀÁ÷ Çö±Ý)"))
					{
						HUD::CHANGE_FAKE_MP_CASH(g_gui.fakeamount, 0);
					}
					ImGui::SameLine();
					if (ImGui::Button("ÆäÀÌÅ© ¸Ó´Ï ¾ò±â (¿ÀÁ÷ ÀºÇà)"))
					{
						HUD::CHANGE_FAKE_MP_CASH(0, g_gui.fakeamount);
					}
					ImGui::RadioButton("ÀüºÎ¾ò±â", &g_gui.fakemoneymode, 1);
					ImGui::SameLine();
					ImGui::RadioButton("¿ÀÁ÷ Çö±Ý", &g_gui.fakemoneymode, 2);
					ImGui::SameLine();
					ImGui::RadioButton("¿ÀÁ÷ ÀºÇà", &g_gui.fakemoneymode, 3);
					ImGui::SameLine();
					ImGui::Checkbox("¼ö·® ·£´ýÈ­", &g_gui.fakeamount_randomize);
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("¼±ÅÃÇÑ ÆäÀÌÅ© ¸Ó´Ï ¾×¼ö¸¦ 2·Î ³ª´« ¼ö¿Í ¼±ÅÃÇÑ ¾×¼ö »çÀÌÀÇ\n·£´ýÇÑ °ªÀ» ÃßÃâÇÏ¿© Ãß°¡ÇÕ´Ï´Ù.");
					}
					ImGui::SameLine();
					ImGui::Checkbox("Ã³¸®Áß Ç¥½Ã", &g_gui.fake_transaction);
					ImGui::SliderInt("·çÇÁ µô·¹ÀÌ", &fake_spam_time_delay_amount, 1, 5000, "%d%ms");
					ImGui::Checkbox("ÆäÀÌÅ© ¸Ó´Ï ·çÇÁ", &g_gui.fakemoney);

					ImGui::BulletText("Ä¿½ºÅÒ ·çÇÁ µô·¹ÀÌ");

					ImGui::SliderInt("½ºÅÚ½º ·çÇÁ#1 µô·¹ÀÌ", &g_money.customdelay700, 1, 5000, "%d%ms");
					ImGui::SliderInt("½ºÅÚ½º ·çÇÁ#2 ¹× #3 µô·¹ÀÌ", &g_money.customdelay1000, 1, 5000, "%d%ms");

					
					ImGui::Checkbox("Partner Panel", &partnerwindow);

				}
			    */
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), reinterpret_cast <const char*>(u8"공식 파트너 | 귀하의 지원에 감사드립니다. | Partner Panel Hotkey = F3"));
			}
			else
			{
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), reinterpret_cast<const char*>(u8"The new Era of Recovery | Redich Group"));
			}
			ImGui::Text(skCrypt("Your encoded_secure_personal_identification_key_seed: %s"), key.substr(key.length() - 6));
			ImGui::Text(skCrypt("Your encoded_secure_personal_identification_key: %s"), securekey_encode);
			ImGui::Text(skCrypt("Your secure_personal_identification_code: %s"), external_ip_encode);
			//ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), reinterpret_cast<const char*>(u8"Your encoded_secure_personal_identification_key: %s", securekey_encode));
		}
		ImGui::End();
    }

	DrawNotifications();
}

void Gui::Initialize() {
	ImGuiIO io = ImGui::GetIO();

    ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
    static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	titleFont = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(PretendardBold), sizeof(PretendardBold), 14.5f, &config, ImGui::GetIO().Fonts->GetGlyphRangesKorean());
    textFont = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(PretendardBold), sizeof(PretendardBold), 17.5f, &config, ImGui::GetIO().Fonts->GetGlyphRangesKorean());
    iconFont = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(PretendardBold), sizeof(PretendardBold), 17.5f, &config, ImGui::GetIO().Fonts->GetGlyphRangesKorean());

	ImVec4* colors = ImGui::GetStyle().Colors;
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}

void Gui::Uninitialize() {
    opened = false;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (context) { context->Release(); context = NULL; }
	if (device) { device->Release(); device = NULL; }
	SetWindowLongPtr(Gui::Instance()->hwnd, GWLP_WNDPROC, (LONG_PTR)(Hooks::Instance()->originalWndProc));
}

void Gui::DrawChild(ImVec2 size, ImVec2 position, const char* text) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.f, 50.f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15.f);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0627f, 0.0706f, 0.0745f, 1.0000f));
	ImVec2 windowPosition = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::SetNextWindowPos(ImVec2(windowPosition.x + position.x, windowPosition.y + position.y));
	ImGui::BeginChild(text, size, ImGuiChildFlags_Border);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	ImVec2 childPosition = ImGui::GetWindowPos();
	ImVec2 childSize = ImGui::GetWindowSize();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	drawList->AddRectFilled(childPosition, ImVec2(childPosition.x + childSize.x, childPosition.y + 40.f), ImColor(27, 29, 33, 255));
	drawList->AddText(ImVec2(childPosition.x + (childSize.x - ImGui::CalcTextSize(text).x) / 2, childPosition.y + 10.f), ImColor(255, 255, 255, 255), text); 
}

void Gui::EndChild() {
	ImGui::EndChild();
}

void Gui::AddNotification(const char* format, ...) {
	char notificationText[256];

	va_list args;
	va_start(args, format);
	vsnprintf(notificationText, sizeof(notificationText), format, args);

	for (auto pair : notifications) {
		if (pair.first == std::string(notificationText)) {
			pair.second = GetTickCount64();
			return;
		}
	}

	notifications.push_back(std::make_pair(std::string(notificationText), GetTickCount64()));

	va_end(args);
}

void Gui::DrawNotifications() {
	for (int i = 0; i < (int)notifications.size(); i++) {
		if (GetTickCount64() - notifications[i].second >= 4000) {
			notifications.erase(notifications.begin() + i);
			continue;
		}

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;

		float width = ImGui::CalcTextSize(notifications[i].first.c_str()).x + 220.f;
		float positionY = 30.f + (i * 43.f);
		float barWidth = width - (float)(((double)(GetTickCount64() - notifications[i].second) / 4000) * width);

		ImDrawList* drawList = ImGui::GetForegroundDrawList();
		drawList->AddRectFilled(
			ImVec2(screenSize.x - width, positionY),
			ImVec2(screenSize.x, positionY + 35.f),
			ImColor(27,29,33,255)
		);
		drawList->AddRectFilled(
			ImVec2(screenSize.x - width, positionY),
			ImVec2(screenSize.x - width + 5.f, positionY + 35.f),
			ImColor(255,255,0,255)
		);
		drawList->AddRectFilled(
			ImVec2(screenSize.x - width, positionY + 30.f),
			ImVec2(screenSize.x - width + barWidth, positionY + 35.f),
			ImColor(255, 255, 0, 255)
		);

		ImGui::PushFont(textFont);
		drawList->AddText(
			ImVec2(screenSize.x - width + 25.f, positionY + 4.f),
			ImColor(255, 255, 255, 255),
			notifications[i].first.c_str()
		);
		ImGui::PopFont();
	}
}