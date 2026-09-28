#include "SDK/SDK.hpp"
#include "framework.h"
#include "Hooks.h"
#include "minhook/minhook.h"

using namespace SDK;

DWORD WINAPI Main(LPVOID)
{
    AllocConsole();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Gameserver - Podge");

    for (auto& Offset : Sarah::Offsets::NullFuncs) {
        HookThings(Offset + ImageBase, Patches::ReturnHook, nullptr);
    }

    HookThings(Sarah::Offsets::TickFlush + ImageBase, Hooks::TickFlush, (void**)&Hooks::TickFlush_OG);
    HookThings(Sarah::Offsets::KickPlayer + ImageBase, Patches::ReturnTrue, nullptr);
    HookThings(Sarah::Offsets::GetNetMode + ImageBase, Patches::ReturnTrue, nullptr);

    HookThings(Sarah::Offsets::ReadyToStartMatch + ImageBase, Hooks::ReadyToStartMatch, nullptr);
    HookThings(Sarah::Offsets::SpawnDefaultPawnFor + ImageBase, Hooks::SpawnDefaultPawnFor, nullptr);

    *(bool*)(Sarah::Offsets::GIsClient + ImageBase) = false;
    *(bool*)(Sarah::Offsets::GIsServer + ImageBase) = true;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(nullptr, 0, Main, nullptr, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}