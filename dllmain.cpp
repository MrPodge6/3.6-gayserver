
#include "pch.h"


DWORD WINAPI Main(LPVOID)
{
	AllocConsole();
	FILE* File = nullptr;
	freopen_s(&File, "CONOUT$", "w", stdout);
	SetConsoleTitleA("Gameserver - Podge");

    return 0
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

