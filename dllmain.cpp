#include "SDK/SDK.hpp"
#include "framework.h"
#include "Hooks.h"

using namespace std;
using namespace SDK;

DWORD WINAPI Main(LPVOID)
{
    AllocConsole();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Gameserver - Podge");

    *(bool*)(Sarah::Offsets::GIsClient) = false;
    *(bool*)(Sarah::Offsets::GIsServer) = true;

    for (auto& Offset : Sarah::Offsets::NullFuncs) {
        Hook(Offset + ImageBase, Patches::ReturnHook, 0);
    }

    Hook(Sarah::Offsets::TickFlush + ImageBase, Hooks::TickFlush, (void**)&Hooks::TickFlush_OG);
    Hook(Sarah::Offsets::KickPlayer + ImageBase, Patches::ReturnTrue, 0);
    Hook(Sarah::Offsets::GetNetMode + ImageBase, Patches::ReturnTrue, 0);

    Hook(Sarah::Offsets::ReadyToStartMatch + ImageBase, Hooks::ReadyToStartMatch, 0);
    Hook(Sarah::Offsets::SpawnDefaultPawnFor + ImageBase, Hooks::SpawnDefaultPawnFor, 0);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    }
    return TRUE;
}