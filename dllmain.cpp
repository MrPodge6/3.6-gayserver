

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
    
    Hook(Sarah::Offsets::KickPlayer, Patches::ReturnTrue, 0);
    Hook(Sarah::Offsets::GetNetMode, Patches::ReturnTrue, 0);

    Hook(Sarah::Offsets::ReadyToStartMatch + ImageBase, Hooks::ReadyToStartMatch, 0);
	Hook(Sarah::Offsets::SpawnDefaultPawnFor + ImageBase, Hooks::SpawnDefaultPawnFor, 0);   

    UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"Open Athena_Terrain", nullptr);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
		CreateThread(0, 0, Main, 0, 0, 0);
        break;
    }
    return TRUE;
}

