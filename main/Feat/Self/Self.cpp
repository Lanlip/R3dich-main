#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "../Fonts/fonts.hpp"
#include "../utility/rage/joaat.hpp"
#include "Self.hpp"
#include "../script_global/script_global.hpp"
#include "../source/script/script.hpp"

int spam_event_time_drop = 0;
void RequestControlOfid(Entity netid)
{
	int tick = 0;

	while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
		tick++;
	}
}

void RequestControlOfEnt(Entity entity)
{
	int tick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		tick++;
	}
	if (NETWORK::NETWORK_IS_SESSION_STARTED())
	{
		int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
		RequestControlOfid(netID);
		NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
	}
}

void ApplyForceToEntity(Entity e, float x, float y, float z)
{
	if (e != PLAYER::PLAYER_PED_ID() && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(e) == FALSE)
	{
		RequestControlOfEnt(e);
	}

	//ENTITY::APPLY_FORCE_TO_ENTITY(e, 1, x, y, z, 0, 0, 0, 0, 1, 1, 1, 0, 1);
}

double DegreeToRadian(double n)
{
	return n * 0.017453292519943295;
}

NativeVector RotationToDirection(NativeVector& rot)
{
	double num = DegreeToRadian(rot.z);
	double num2 = DegreeToRadian(rot.x);
	double val = cos(num2);
	double num3 = abs(val);
	rot.x = (float)(-(float)sin(num) * num3);
	rot.y = (float)(cos(num) * num3);
	rot.z = (float)sin(num2);
	return rot;

}

NativeVector addVectors(NativeVector vector, NativeVector vector2)
{
	vector.x += vector2.x;
	vector.y += vector2.y;
	vector.z += vector2.z;
	return vector;
}

NativeVector multiplyVector(NativeVector vector, float inc)
{
	vector.x *= inc;
	vector.y *= inc;
	vector.z *= inc;

	return vector;
}

void self::teleport_to_coords(Entity e, NativeVector coords)
{
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
}

using Blip = std::int32_t;
NativeVector self::get_blip_marker()
{
	static NativeVector zero;
	NativeVector coords;

	bool blipFound = false;
	int blipIterator = HUD::GET_WAYPOINT_BLIP_ENUM_ID();
	for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator); HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator))
	{
		if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4)
		{
			coords = HUD::GET_BLIP_INFO_ID_COORD(i);
			blipFound = true;
			break;
		}
	}
	if (blipFound)
	{
		return coords;
	}

	return zero;
}

void self::teleport_to_markerr()
{
	NativeVector coords = g_self.get_blip_marker();

	if (coords.x == 0 && coords.y == 0)
	{
		return;
	}

	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
	{
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	}

	bool groundFound = false;
	static float groundCheckHeight[] = { 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
	for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
		//Script::Current()->ScriptYield();
		if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0, 0))
		{
			groundFound = true;
			coords.z += 3.0;
			break;
		}
	}

	if (!groundFound)
	{
		coords.z = 1000.0;
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
	}

	teleport_to_coords(e, coords);
}

bool spawn_in_vehicle = false;
bool engine_on = false;
bool spawn_god = false;

float degToRad(float degs)
{
	return degs * 3.141592653589793f / 180.f;
}

void self::spawn_vehiclesss(char* toSpawn) {
	Queue::Instance()->Add([=] {
		Hash model = MISC::GET_HASH_KEY(toSpawn);
		STREAMING::REQUEST_MODEL(model);
		NativeVector ourCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);
		float forward = 5.f;
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		float xVector = forward * sin(degToRad(heading)) * -1.f;
		float yVector = forward * cos(degToRad(heading));
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x + xVector, ourCoords.y + yVector, ourCoords.z, heading, true, true, true);
		RequestControlOfEnt(veh);
		if (g_self.spawn_in_vehicle)
		{
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		}
		if (g_self.engine_on)
		{
			VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
		}
		if (g_self.spawn_god)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(veh, true);
		}
	});
}

void self::self_feattt()
{
	if (g_self.godmode)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			auto vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			ENTITY::SET_ENTITY_INVINCIBLE(vehicle, true);
		}
	}
	else
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			auto vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			ENTITY::SET_ENTITY_INVINCIBLE(vehicle, false);
		}
	}

	if (g_self.neverwanted)
	{
		Player player = PLAYER::PLAYER_PED_ID();
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
		PLAYER::SET_MAX_WANTED_LEVEL(0);
		PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
		PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
		PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
		PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);
	}
	else
	{
		Player player = PLAYER::PLAYER_PED_ID();
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		PLAYER::SET_POLICE_IGNORE_PLAYER(player, false);
		PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
		PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
		PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);
	}

	if (g_self.noragdoll)
	{
		PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), false);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), false);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), false);
		PLAYER::GIVE_PLAYER_RAGDOLL_CONTROL(PLAYER::PLAYER_ID(), true);
		PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), false);
	}

	if (g_self.invisible)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false, 0);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true, 0);
	}

	if (g_self.noidlekick)
	{
		*script_global(1668667 + 1154).as<int*>() = 0;
		*script_global(1668667 + 1155).as<int*>() = 0;
		*script_global(1668667 + 1170).as<int*>() = 0;
	}

	if (g_self.moneydrop)
	{
		Queue::Instance()->Add([=] {
			if ((MISC::GET_GAME_TIMER() - spam_event_time_drop) > 200)
			{
				STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));

				float randomfo = MISC::GET_RANDOM_FLOAT_IN_RANGE(3.0f, 6.0f);

				while (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble")))
				{
					Script::Current()->ScriptYield(std::chrono::milliseconds(0));
				}

				if (STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble")))
				{
					auto playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);
					OBJECT::CREATE_AMBIENT_PICKUP(-1009939663, playerPosition.x + MISC::GET_RANDOM_FLOAT_IN_RANGE(moneyrainrange * -1, moneyrainrange), playerPosition.y + MISC::GET_RANDOM_FLOAT_IN_RANGE(moneyrainrange * -1, moneyrainrange), playerPosition.z + 5.f, 0, 1, MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"), FALSE, TRUE);
					OBJECT::CREATE_AMBIENT_PICKUP(-1009939663, playerPosition.x + MISC::GET_RANDOM_FLOAT_IN_RANGE(moneyrainrange * -1, moneyrainrange), playerPosition.y + MISC::GET_RANDOM_FLOAT_IN_RANGE(moneyrainrange * -1, moneyrainrange), playerPosition.z + 5.f, 0, 1, MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"), FALSE, TRUE);
					OBJECT::CREATE_AMBIENT_PICKUP(-1009939663, playerPosition.x + MISC::GET_RANDOM_FLOAT_IN_RANGE(moneyrainrange * -1, moneyrainrange), playerPosition.y + MISC::GET_RANDOM_FLOAT_IN_RANGE(moneyrainrange * -1, moneyrainrange), playerPosition.z + 5.f, 0, 1, MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"), FALSE, TRUE);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));
					Script::Current()->ScriptYield(std::chrono::milliseconds(200));
				}
				spam_event_time_drop = MISC::GET_GAME_TIMER();
			}
		});
	}

	if (g_self.superun)
	{
		Ped ped = PLAYER::PLAYER_PED_ID();
		if (TASK::IS_PED_RUNNING(ped) || TASK::IS_PED_SPRINTING(ped))
		{
			//ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.f, 1.5f, 0.f, 0.f, 0.f, 0.f, 1, true, true, true, false, true); 병신베이스
		}
	}

	if (g_self.superman)
	{
		/*
		Queue::Instance()->Add([] {
			if (GetAsyncKeyState(VK_SPACE))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0, 0, 10, 0, 0, 0, 1, true, true, true, true, true);
			}
			Hash hash = MISC::GET_HASH_KEY("GADGET_PARACHUTE");
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 1, 1);

			if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
			{
				if (GetAsyncKeyState(0x57)) // W key
				{
					ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 0, 0);
				}

				if (GetAsyncKeyState(0x53)) // S key
				{
					ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 6, 0);
				}
				if (GetAsyncKeyState(VK_SHIFT))
				{
					ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 6, 0, 0);
				}
			}
		});
		*/
	}

	if (g_self.noclip)
	{
		auto ped = PLAYER::PLAYER_PED_ID();
		auto xyzcoords = ENTITY::GET_ENTITY_COORDS(ped, true);
		auto heading = CAM::GET_GAMEPLAY_CAM_RELATIVE_HEADING() + ENTITY::GET_ENTITY_HEADING(ped);
		auto pitch = CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH();

		auto speed = 1.0f;

		auto xm = -sin(heading * 3.14159265358979323846 / 180.0);
		auto ym = cos(heading * 3.14159265358979323846 / 180.0);
		auto zm = sin(pitch * 3.14159265358979323846 / 180.0);

		auto len = sqrt(xm * xm + ym * ym + zm * zm);
		if (len != 0)
		{
			xm = xm / len;
			ym = ym / len;
			zm = zm / len;
		}

		auto x = xyzcoords.x;
		auto y = xyzcoords.y;
		auto z = xyzcoords.z;

		ENTITY::SET_ENTITY_VELOCITY(ped, 0.0001, 0.0001, 0.0001);
		if (GetAsyncKeyState(VK_SPACE) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 21))
		{
			speed = speed + 5;
		}
		if (GetAsyncKeyState(0x57))
		{
			x = x + speed * xm;
			y = y + speed * ym;
			z = z + speed * zm;
		}
		if (GetAsyncKeyState(0x53))
		{
			x = x - speed * xm;
			y = y - speed * ym;
			z = z - speed * zm;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
		{
			auto vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
			ENTITY::SET_ENTITY_HEADING(vehicle, heading);
			ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(vehicle, false, true);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(vehicle, x, y, z, true, true, true);
		}
		else
		{
			ENTITY::SET_ENTITY_HEADING(ped, heading);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, x, y, z, true, true, true);
		}
	}
	else
	{
		auto ped = PLAYER::PLAYER_PED_ID();
		auto vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
		ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(vehicle, true, true);
	}


	if (g_self.infinityammo)
	{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), true);
	}
	else
	{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), false);
	}

	if (g_self.moneyammo)
	{
		Queue::Instance()->Add([] {
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));

				Player playerPed = PLAYER::PLAYER_PED_ID();
				if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
					PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
					auto gameplayCam = CAM::GET_GAMEPLAY_CAM_COORD();
					auto gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
					auto gameplayCamDirection = RotationToDirection(gameplayCamRot);
					NativeVector startCoords = addVectors(gameplayCam, (multiplyVector(gameplayCamDirection, 1.0f)));
					Hash weaponhash;
					WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, 1);
					if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
						OBJECT::CREATE_AMBIENT_PICKUP(-1009939663, startCoords.x, startCoords.y, startCoords.z + 0.4f, 0, 1, MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"), FALSE, TRUE);
						Script::Current()->ScriptYield(std::chrono::milliseconds(100));
					}

				}
			}
		});
	}

	if (g_self.rapidfire)
	{
		Queue::Instance()->Add([] {
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				Player playerPed = PLAYER::PLAYER_PED_ID();
				if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
					PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
					auto gameplayCam = CAM::GET_GAMEPLAY_CAM_COORD();
					auto gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
					auto gameplayCamDirection = RotationToDirection(gameplayCamRot);
					NativeVector startCoords = addVectors(gameplayCam, (multiplyVector(gameplayCamDirection, 1.0f)));
					NativeVector endCoords = addVectors(startCoords, multiplyVector(gameplayCamDirection, 500.0f));
					Hash weaponhash;
					WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, 1);
					if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, PLAYER::PLAYER_PED_ID(), 1, 1, 0xbf800000);
						//MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, 100, true, weaponhash, PLAYER::PLAYER_PED_ID(), true, false, 100);
					}

				}
			}
		});
	}

	if (g_self.explodeammo)
	{
		if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && PED::IS_ANY_PED_SHOOTING_IN_AREA)
		{
			NativeVector iCoord;

			//if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
			//{
			//	Logger::Instance()->Log("DEBUG", "%s %s %s", iCoord.x, iCoord.y, iCoord.z);
			//	FIRE::ADD_EXPLOSION(iCoord.x, iCoord.y, iCoord.z, 25, 10000.0f, true, false, 0, false);
			//}
		}
	}
}

/*
Ped playerPed = PLAYER::PLAYER_PED_ID();

if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_SHOOTING_IN_AREA)
{
	Vector3 coords;
	if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &coords))
	{
		STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));
		if (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"))) {
			//script::get_current()->yield(0ms);
		}
		else
		{
			OBJECT::CREATE_AMBIENT_PICKUP(-1009939663, coords.x, coords.y, coords.z + 0.4f, 0, 1, MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"), FALSE, TRUE);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));
		}
	}

}
*/