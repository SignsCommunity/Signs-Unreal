// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "MainSign.h"
#include "PlayerCharacter.generated.h"

/**
 * Contains logic for the player character we controll.
 * It is extended in a Blueprint where the visual componenets are attached.
 * It handles spawning of a the sign as well as replicating its reference.
 */
UCLASS()
class SIGNS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Sign")
	class AMainSign* MainSignRef;

	TSubclassOf<class AMainSign> MainSignBPRef;

	virtual void NotifyHit
	(
		class UPrimitiveComponent * MyComp,
		AActor * Other,
		class UPrimitiveComponent * OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult & Hit
	);

private:

	/* Enables all physics properties of the CapsuleComponent */
	void EnablePhysics();
	
	/* Disables all physics properties of the CapsuleComponent */
	void DisablePhysics();

	UPROPERTY(EditAnywhere)
	float BounceTime = 2.0f;

	FTimerHandle FiredTimerHandle;

	/* [Server] Spawns MainSign on BeginPlay */
	void SpawnSign();
};
