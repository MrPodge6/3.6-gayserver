#pragma once
#include "framework.h"
#include "Offsets.h"


namespace Hooks
{
	bool ReadyToStartMatch(AFortGameModeAthena* thisptr) {
		static int NumberOfCalls = 0;
		AFortGameStateAthena* GameState = (AFortGameStateAthena*)thisptr->GameState;

		if (!GameState || GameState->MapInfo)
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

			thisptr->WarmupRequiredPlayerCount = 1;
			
			NumberOfCalls++;

		}
		if (NumberOfCalls == 1) {
			// use Sarah::Funcs we dont need to make a funcs file
			auto GameNetDriverName = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
			UNetDriver* NetDriver = Sarah::Funcs::CreateNetDriver(UEngine::GetEngine(), UWorld::GetWorld(), UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"));
			NetDriver->World = UWorld::GetWorld();
			NetDriver->NetDriverName = GameNetDriverName;

			FURL URL{};
			URL.Port = 7777;

			FString TemporaryString;
			Sarah::Funcs::InitListen(NetDriver, UWorld::GetWorld(), URL, false, TemporaryString);
			Sarah::Funcs::SetWorld(NetDriver, UWorld::GetWorld());

			for (auto& LevelCollection : UWorld::GetWorld()->LevelCollections) {
				LevelCollection.NetDriver = NetDriver;
			}

			SetConsoleTitleA("Listening is now on - Podges Gayserver");


		}
		
		return thisptr->AlivePlayers.Num() >= thisptr->WarmupRequiredPlayerCount;

	}
	APawn* SpawnDefaultPawnFor(AFortGameModeAthena* GameMode, AFortPlayerControllerAthena* Controller, AActor* StartSpot) {
		return GameMode->SpawnDefaultPawnAtTransform(Controller, StartSpot->GetTransform());
	}

	inline void (*TickFlush_OG)(UNetDriver* NetDriver, float DeltaSeconds);
	void TickFlush(UNetDriver* Driver, float DeltaSeconds)
		//ServerReplicateActors
	{
		if (Driver->ReplicationDriver) {
			// ServerReplicateActors(Driver->ReplicationDriver, DeltaSeconds); i cant find the fucking offset pls tell me what it is :`(

		}

		return TickFlush_OG(Driver, DeltaSeconds);
	}

	
}

namespace Patches
{
	int ReturnTrue() {
		return 1;
	}
}