#include "SDK/SDK.hpp"
#include "framework.h"
#include "Hooks.h"
#include "minhook/minhook.h"

using namespace std;
using namespace SDK;

DWORD WINAPI Main(LPVOID)
{
    AllocConsole();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Gameserver - Podge");

    for (auto& Offset : Sarah::Offsets::NullFuncs) {
        Hook(Offset + ImageBase, Patches::ReturnHook, nullptr);
    }

    Hook(Sarah::Offsets::TickFlush + ImageBase, Hooks::TickFlush, (void**)&Hooks::TickFlush_OG);
    Hook(Sarah::Offsets::KickPlayer + ImageBase, Patches::ReturnTrue, nullptr);
    Hook(Sarah::Offsets::GetNetMode + ImageBase, Patches::ReturnTrue, nullptr);

    Hook(Sarah::Offsets::ReadyToStartMatch + ImageBase, Hooks::ReadyToStartMatch, nullptr);
    Hook(Sarah::Offsets::SpawnDefaultPawnFor + ImageBase, Hooks::SpawnDefaultPawnFor, nullptr);

    *(bool*)(Sarah::Offsets::GIsClient) = false;
    *(bool*)(Sarah::Offsets::GIsServer) = true;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        std::thread(Main).detach();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
