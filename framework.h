#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <memory>
#include "SDK/SDK.hpp"

using namespace std;
using namespace SDK;

#include "minhook/MinHook.h"
#include "Offsets.h"


static void HookThings(uint64_t Offsets, PVOID Hook, void** Detour)
{
	static bool bInitialized = false;
	if (!bInitialized) {
		MH_Initialize();
		bInitialized = true;
	}
	MH_CreateHook((PVOID)Offsets, Hook, Detour);
	MH_EnableHook((PVOID)Offsets);
}

#define Hook(...)HookThings(__VA_ARGS__);