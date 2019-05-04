// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TimerManager.h"
#include "SimpleGameMode.generated.h"

/**
 * There is a single GameMode and it exists on the server only.
 * It knows about the player start positions and their availability.
 * Its a single authoritative source of truth.
 */
UCLASS()
class SIGNS_API ASimpleGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void PostLogin(APlayerController * PlayerJoined) override;

	FVector GetPlayerSpawnPosition(APlayerController *PC, const FName &Team);

private:

	TArray<AActor*> PlayerStarts;
	TArray<bool> PlayerStartAvailability;
};
