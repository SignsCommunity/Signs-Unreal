// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <EngineClasses.h>
#include "MainSign.generated.h"

/**
 * Represents the state of the actor
 */
UENUM(BlueprintType)
enum class EStateEnum : uint8 {
	FIRED       UMETA(DisplayName = "Fired"),
	ROTATING    UMETA(DisplayName = "Rotating"),
	IDLE        UMETA(DisplayName = "Idle")
};

UCLASS()
class SIGNS_API AMainSign : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainSign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    /** Creates and initializes the components of the actor */
	void InitComponents();

	float RunningTime;

	EStateEnum State;

	FTimerHandle FiredTimerHandle;

	/**
	 * Calculates the next location of the actor based on the "rotating" logic
	 * @param DeltaTime - taken from Tick()
	 */
	void ContinueOrbitalPath(float DeltaTime);

	UFUNCTION()
	void ReturnToOrbit();

	/** The time that the sign is in a fired state (in seconds) */
	UPROPERTY(EditAnywhere, Category = "FiredState")
	float MaxFiredTime;

	UPROPERTY(EditAnywhere, Category = "OrbitalBehaviour")
	float OrbitalRadius;

	UPROPERTY(EditAnywhere, Category = "OrbitalBehaviour")
	float AngularVelocity;

	UPROPERTY(EditAnywhere, Category = "OrbitalBehaviour")
	float StartAngle;

	UPROPERTY(EditAnywhere, Category = "OrbitalBehaviour")
	float RelativeHeight;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	float SignCoreInnerRadius;

	//Components
	UPROPERTY(EditAnywhere, Category = "Appearance")
	USphereComponent *SignCoreComponent;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	UStaticMeshComponent *SignCoreVisual;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	UParticleSystemComponent *TrailParticleSystem;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent *MovementComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Checks if the actor can be fired and if so it executes the "fire" logic
	 * @param Direction - The difference between the cursor location and the player (as vector)
	 */
	void TryFire(FVector Direction);
};
