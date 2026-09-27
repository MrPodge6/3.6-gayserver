#pragma once
#include "framework.h"
#include "Offsets.h"


namespace Hooks
{
	bool ReadyToStartMatch(AFortGameModeAthena* thisptr) {
		static int NumberOfCalls = 0; // keep updating ts
		AFortGameStateAthena* GameState = (AFortGameStateAthena*)thisptr->GameState;

		if (!GameState || GameState->MapInfo) // if the map doesnt load or doesnt have Gamestate will just return readytostartmatch as false
			return false;

		if (NumberOfCalls == 0) {
			UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");
			GameState->CurrentPlaylistData = Playlist;

			// reminder: Podge do not use this s6 or above as it gotta be different or it fucking breaks the gayserver


			thisptr->CurrentPlaylistId = Playlist->PlaylistId;
			thisptr->CurrentPlaylistName = Playlist->PlaylistName;
			GameState->CurrentPlaylistId = Playlist->PlaylistId;

			GameState->OnRep_CurrentPlaylistData();
			GameState->OnRep_CurrentPlaylistId();

			thisptr->WarmupRequiredPlayerCount = 1; // change to ur liking ig

			NumberOfCalls++;

		}
		if (NumberOfCalls == 1) {
			// use Sarah::Funcs we dont need to make a funcs file
			auto GameNetDriverName = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
			UNetDriver* NetDriver = Sarah::Funcs::CreateNetDriver(UEngine::GetEngine(), UWorld::GetWorld(), UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"));
			NetDriver->World = UWorld::GetWorld();
			NetDriver->NetDriverName = GameNetDriverName;

			FURL URL{};
			URL.Port = 7777; // port of the gs

			FString TemporaryString;
			Sarah::Funcs::InitListen(NetDriver, UWorld::GetWorld(), URL, false, TemporaryString);
			Sarah::Funcs::SetWorld(NetDriver, UWorld::GetWorld());

			for (auto& LevelCollection : UWorld::GetWorld()->LevelCollections) {
				LevelCollection.NetDriver = NetDriver; // important so it adds a valid netdriver to each level
			}

			thisptr->bWorldIsReady = true;

			SetConsoleTitleA("Listening is now on - Podges Gayserver");
			NumberOfCalls++;


		}

		return thisptr->AlivePlayers.Num() >= thisptr->WarmupRequiredPlayerCount; // returns true if there is the same or more players than required to start warmup (spawn island)

	}
	APawn* SpawnDefaultPawnFor(AFortGameModeAthena* GameMode, AFortPlayerControllerAthena* Controller, AActor* StartSpot) {
		return GameMode->SpawnDefaultPawnAtTransform(Controller, StartSpot->GetTransform()); // spawns the player i think at the start spot location??
	}

	inline void (*TickFlush_OG)(UNetDriver* NetDriver, float DeltaSeconds);
	void TickFlush(UNetDriver* Driver, float DeltaSeconds) // does ticks

	{
		static bool bDidLevelSwitch = false;
		if (!bDidLevelSwitch) {
			auto World = UWorld::GetWorld();
			if (World && World->OwningGameInstance && World->OwningGameInstance->LocalPlayers.Num() > 0) {
				auto GameInstance = World->OwningGameInstance;
				auto LocalPlayer = GameInstance->LocalPlayers[0];
				if (LocalPlayer && LocalPlayer->PlayerController) {
					LocalPlayer->PlayerController->SwitchLevel(L"Athena_Terrain");
				}
				while (GameInstance->LocalPlayers.Num() > 0) {
					GameInstance->LocalPlayers.Remove(0);
				}
				bDidLevelSwitch = true;
			}
		}

		if (Driver->ReplicationDriver) {
			Sarah::Funcs::ServerReplicateActors(Driver->ReplicationDriver, DeltaSeconds); //i found the offset finally yes. Updates the client i believe?

		}

		return TickFlush_OG(Driver, DeltaSeconds);
	}


}

namespace Patches
{
	int ReturnTrue() {
		return 1; // easy function to use fast
	}

	void ReturnHook() {
		return; // returns the hook
	}
}