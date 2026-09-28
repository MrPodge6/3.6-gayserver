#pragma once
#include "framework.h"
#include "Offsets.h"


namespace Hooks
{
	bool ReadyToStartMatch(AFortGameModeAthena* thisptr) {
		static int NumberOfCalls = 0; // keep updating ts
		
		if (!thisptr || !thisptr->GameState)
			return false;

		AFortGameStateAthena* GameState = (AFortGameStateAthena*)thisptr->GameState;

		if (NumberOfCalls == 0) {
			UWorld* World = UWorld::GetWorld();
			if (World && World->OwningGameInstance && World->OwningGameInstance->LocalPlayers.Num() > 0) {
				World->OwningGameInstance->LocalPlayers.Remove(0); // will see if this actually works later.
			}

			UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");
			if (Playlist) {
				GameState->CurrentPlaylistData = Playlist;

				// reminder: Podge do not use this s6 or above as it gotta be different or it fucking breaks the gayserver


				thisptr->CurrentPlaylistId = Playlist->PlaylistId;
				thisptr->CurrentPlaylistName = Playlist->PlaylistName;
				GameState->CurrentPlaylistId = Playlist->PlaylistId;

				GameState->OnRep_CurrentPlaylistData();
				GameState->OnRep_CurrentPlaylistId();
			}

			thisptr->WarmupRequiredPlayerCount = 1; // change to ur liking ig

			NumberOfCalls++;

		}
		if (NumberOfCalls == 1) {
			// use Sarah::Funcs we dont need to make a funcs file
			UWorld* World = UWorld::GetWorld();
			auto GameNetDriverName = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
			UNetDriver* NetDriver = Sarah::Funcs::CreateNetDriver(UEngine::GetEngine(), World, UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"));
			
			if (NetDriver) {
				NetDriver->World = World;
				NetDriver->NetDriverName = GameNetDriverName;

				FURL URL{};
				URL.Port = 7777; // port of the gs

				FString TemporaryString;
				Sarah::Funcs::InitListen(NetDriver, World, URL, false, TemporaryString);
				Sarah::Funcs::SetWorld(NetDriver, World);

				for (auto& LevelCollection : World->LevelCollections) {
					LevelCollection.NetDriver = NetDriver; // important so it adds a valid netdriver to each level
				}

				thisptr->bWorldIsReady = true;

				SetConsoleTitleA("Listening is now on - Podges Gayserver");
				NumberOfCalls++;
			}


		}

		return thisptr->AlivePlayers.Num() >= thisptr->WarmupRequiredPlayerCount; // returns true if there is the same or more players than required to start warmup (spawn island)

	}
	APawn* SpawnDefaultPawnFor(AFortGameModeAthena* GameMode, AFortPlayerControllerAthena* Controller, AActor* StartSpot) {
		return GameMode->SpawnDefaultPawnAtTransform(Controller, StartSpot->GetTransform()); // spawns the player i think at the start spot location??
	}

	inline void (*TickFlush_OG)(UNetDriver* NetDriver, float DeltaSeconds);
	void TickFlush(UNetDriver* Driver, float DeltaSeconds) // does ticks

	{
		

		if (Driver && Driver->ReplicationDriver) {
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
