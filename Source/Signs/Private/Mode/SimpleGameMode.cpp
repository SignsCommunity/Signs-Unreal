// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleGameMode.h"

#include "SimplePlayerController.h"
#include <EngineGlobals.h>
#include "Signs.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>

void ASimpleGameMode::PostLogin(APlayerController * PlayerJoined)
{
	Super::PostLogin(PlayerJoined);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	PlayerStartAvailability.Init(true, PlayerStarts.Num());
}


FVector ASimpleGameMode::GetPlayerSpawnPosition(APlayerController *PC, const FName &Team)
{
	for (uint8 i = 0; i < PlayerStarts.Num(); i++)
	{
		if (PlayerStarts[i]->ActorHasTag(Team) && PlayerStartAvailability[i]) {
			PlayerStartAvailability[i] = false;
			return PlayerStarts[i]->GetActorLocation();
		}
	}

	//Dont spawn player if he enters the wrong team name!
	return FVector::ZeroVector;
}
