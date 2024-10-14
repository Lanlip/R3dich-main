#include "hooks.hpp"
#include "MinHook.h"
#include "../utility/logger/logger.hpp"
#include "../utility/signatures/signatures.hpp"
#include <cstring>
#include <iostream>

static VTSwap* vtSwapInstance = nullptr;
static Hooks* hooksInstance = nullptr;

VTSwap* VTSwap::Instance() {
	if (!vtSwapInstance)
		vtSwapInstance = new VTSwap();

	return vtSwapInstance;
}

void VTSwap::Create(void* address, int index, void* detour, void** original, const char* name) {
    void** vtableEntries = *(void***)address;

    DWORD oldProtect;
    VirtualProtect(&vtableEntries[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);

    *original = (void*)vtableEntries[index];
    vtableEntries[index] = detour;

    VirtualProtect(&vtableEntries[index], sizeof(void*), oldProtect, &oldProtect);

    std::tuple<void*, int, void**> insert {address, index, original};

    swapped.emplace_back(insert);


    //char str2[30] = "Created ";

    //strcat(str2, name);

    //Logger::Instance()->Log("Hooks", str2);
}
void VTSwap::Destroy() {
    for (auto unload : swapped) {
        void* address = std::get<0>(unload);
        int index = std::get<1>(unload);
        void** original = std::get<2>(unload);

        void** vtableEntries = *(void***)address;

        DWORD oldProtect;
        VirtualProtect(&vtableEntries[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);

        vtableEntries[index] = *original;

        VirtualProtect(&vtableEntries[index], sizeof(void*), oldProtect, &oldProtect);
    }
}

void Hooks::Detour(uintptr_t address, void* detour, void** original, const char* name) {
    MH_CreateHook(
        (void*)address,
        detour,
        original
    );

   // char str1[30] = "Created ";

    //strcat(str1, name);

    //Logger::Instance()->Log("Hooks", str1);
    MH_EnableHook((void*)address);
}

Hooks* Hooks::Instance() {
	if (!hooksInstance)
		hooksInstance = new Hooks();

	return hooksInstance;
}

void Hooks::Initialize() {
    MH_Initialize();

    IDXGISwapChain* vtable = *Signatures::Instance()->definitions.swapchain;
    VTSwap::Instance()->Create(
        (void*)vtable, 
        8, 
        (void*)&Present, 
        &originalPresent, 
        "PT"
    );

    Detour(
        Signatures::Instance()->definitions.scriptVm,
        &ScriptVm,
        (void**)(&originalScriptVm),
        "SVM"
    );

    Detour(
        (uintptr_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "ConvertThreadToFiber"),
        &ConvertThreadToFiber,
        (void**)(&originalConvertThreadToFiber),
        "CTTF"
    );

    Detour(
        Signatures::Instance()->definitions.onPlayerJoin,
        &OnPlayerJoin,
        (void**)(&originalOnPlayerJoin),
        "OPJ"
    );

    Detour(
        Signatures::Instance()->definitions.onPlayerLeave,
        &OnPlayerLeave,
        (void**)(&originalOnPlayerLeave),
        "OPL"
    );

    Logger::Instance()->Log("Hooks", "Hooks Initialized");
}

void Hooks::Uninitialize() {
    VTSwap::Instance()->Destroy();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    hooksInstance = nullptr;
    delete this;
}