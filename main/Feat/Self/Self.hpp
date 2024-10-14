using Entity = std::int32_t;
using Player = std::int32_t;
using Ped = Entity;
using Hash = std::uint32_t;
using Vehicle = std::uint32_t;

class self
{
public:
    void self_feattt();
    void teleport_to_coords(Entity e, NativeVector coords);
    NativeVector get_blip_marker();
    void teleport_to_markerr();
    void spawn_vehiclesss(char* toSpawn);

public:
    bool spawn_in_vehicle = false;
    bool engine_on = false;
    bool spawn_god = false;
    float moneyrainrange = 10.f;

    bool godmode = false;
    bool neverwanted = false;
    bool noragdoll = false;
    bool invisible = false;
    bool noidlekick = false;

    bool superun = false;
    bool superman = false;
    bool noclip = false;

    bool moneydrop = false;

    bool infinityammo = false;
    bool moneyammo = false;
    bool explodeammo = false;
    bool rapidfire = false;
};
inline self g_self;