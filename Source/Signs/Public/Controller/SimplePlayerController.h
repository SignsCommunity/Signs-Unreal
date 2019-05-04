// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player/PlayerCharacter.h"

#include "SimplePlayerController.generated.h"

/**
 * The PlayerController is owned by the client. (or listen-server)
 * It is where the input is processed first,
 * and where we call Server RPC's to spawn move/act on possessed pawns.
 */
UCLASS()
class SIGNS_API ASimplePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	/* Called from GameInstance. Calls the GameMode to ask for spawn location. */
	UFUNCTION(Server, reliable, WithValidation)
	void ServerSpawnPlayer(const FName &Team);

	FName Team;

private:

	/* The current MouseLocation */
	float LocationX;
	float LocationY;

	/* Moves the player actor to location using the AI Controller. Calls method on the PlayerProxy */
	UFUNCTION(Server, reliable, WithValidation)
	void ServerMoveToLocation(const FVector &HitResultLocation);

	/* Calls method on the Signs to fire in direction. Called only from the client */
	UFUNCTION(Server, reliable, WithValidation)
	void ServerTryFire(const FVector &Direction);

	/* Calls method on the Signs to fire in direction. Called only from the client */
	UFUNCTION(Server, reliable, WithValidation)
	void ServerTryPull();


	UFUNCTION()
	void TryFire();

	UFUNCTION()
	void TryPull();

	UFUNCTION()
	void MoveToLocation();

	FVector GetMouseRayHitLocation();
};
