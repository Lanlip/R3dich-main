#ifndef _FEATURES_HPP
#define _FEATURES_HPP

#include "../utility/rage/rage.hpp"

class Features {
public: 
    void DiscordInit();
    void UpdatePresence();
    void DiscordDestroy();
    bool discord_rpc = true;

    static Features* Instance();
    void Initialize();
    void Uninitialize();
    void Tick();
public:
    int localPed;
    int localPlayer;
    CPedFactory* pedFactory;
    CPed* localPedPointer;
    CPlayerInfo* localPlayerInfo;
};
inline Features g_feat;
#endif