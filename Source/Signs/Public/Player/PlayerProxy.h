// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.h"
#include "AIController.h"
#include "SimplePlayerController.h"
#include "PlayerProxy.generated.h"

/**
 * This is the Default Pawn Class from the GameMode.
 * Its owned by the local Player Controller.
 * Handles spawning of the PlayerCharacter and the AIController on the server.
 * It contains the Camera component. 
 */
UCLASS()
class SIGNS_API APlayerProxy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerProxy();

public:	
	/* It follows the player character */
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/* [Server] Uses the AI Controller to move the player character */
	void MoveToLocation(const FVector &Destination);

	void SpawnPlayer(const FVector &Location);

	UPROPERTY(Replicated)
		class APlayerCharacter * PlayerCharacterRef;

private:

	TSubclassOf<class APlayerCharacter> BP_PlayerCharacterRef;

	class AAIController *AIControllerRef;
};
