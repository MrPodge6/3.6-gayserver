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
			UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");
			GameState->CurrentPlaylistData = Playlist;

			// reminder: Podge do not use this s6 or above as it gotta be different or it fucking breaks the gayserver


			thisptr->CurrentPlaylistId = Playlist->PlaylistId;
			thisptr->CurrentPlaylistName = Playlist->PlaylistName;
			GameState->CurrentPlaylistId = Playlist->PlaylistId;
			
			GameState->OnRep_CurrentPlaylistData();
			GameState->OnRep_CurrentPlaylistId();
			

			NumberOfCalls++;

		}
		if (NumberOfCalls == 1) {
			// use Sarah::Funcs we dont need to make a funcs file
			UNetDriver* NetDriver = Sarah::Funcs::CreateNetDriver(UEngine::GetEngine(), UWorld::GetWorld(), UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"));
			NetDriver->World = UWorld::GetWorld();
		}
	}
}