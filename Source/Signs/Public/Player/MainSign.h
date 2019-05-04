// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class APlayerCharacter;

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
	RETURNING   UMETA(DisplayName = "Returning"),
	IDLE        UMETA(DisplayName = "Idle")
};

/**
 * Logic for the main sign that the player controlls.
 * It has a ProjectileMovementComponent to handle firing the sign.
 */
UCLASS()
class SIGNS_API AMainSign : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainSign();

private:
	/** Creates and initializes the components of the actor */
	void InitComponents();

	/** Initializes the default values for the variables */
	void InitVariables();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	float RunningTime;

	/** It multiples the initial speed. Used for lowering the speed when the sign is getting close to the orbit */
	float CurrentReturnSpeedRatio;

	UPROPERTY(Replicated)
		EStateEnum State;

	FTimerHandle FiredTimerHandle;

	/**
	 * Calculates the next location of the actor based on the ROTATING state logic
	 * @param DeltaTime - taken from Tick()
	 */
	void ContinueOrbitalPath(float DeltaTime);

	/** Changes the state of the sign */
	UFUNCTION()
		void ChangeState();

	/** Returns true if the sign is outside of the orbit circle area */
	UFUNCTION()
		bool IsOutsideOrbit();

	/**
	 * Calculates the path when the sign is in a RETURNING state
	 * @param DeltaTime - taken from Tick()
	 */
	UFUNCTION()
		void UpdateReturnPath(float DeltaTime);

	/** The time that the sign is in a fired state (in seconds) */
	UPROPERTY(EditAnywhere, Category = "FiredState")
		float MaxFiredTime;

	/** Multiplies the velocity on impact with other actor */
	UPROPERTY(EditAnywhere, Category = "FiredState")
		float ImpulseMultiplier;

	/** The radius when the sign is absorbed in the orbit */
	UPROPERTY(EditAnywhere, Category = "ReturnState")
		float PullRadius;

	/** The radius when the speed starts decreasing */
	UPROPERTY(EditAnywhere, Category = "ReturnState")
		float DecreaseSpeedRadius;

	/** The starting speed ratio */
	UPROPERTY(EditAnywhere, Category = "ReturnState")
		float SpeedRatioTopBoundary;

	/** The limit when the speed stops decreasing */
	UPROPERTY(EditAnywhere, Category = "ReturnState")
		float SpeedRatioDownBoundary;

	/** Multiples DeltaTime and subtracts from current speed */
	UPROPERTY(EditAnywhere, Category = "ReturnState")
		float DeltaTimeMultiplier;

	UPROPERTY(EditAnywhere, Category = "RotatingState")
		float OrbitalRadius;

	UPROPERTY(EditAnywhere, Category = "RotatingState")
		float AngularVelocity;

	UPROPERTY(EditAnywhere, Category = "RotatingState")
		float StartAngle;

	UPROPERTY(EditAnywhere, Category = "RotatingState")
		float RelativeHeight;

	UPROPERTY(EditAnywhere, Category = "Appearance")
		float SignCoreInnerRadius;

	//Components
	UPROPERTY(EditAnywhere, Category = "Appearance")
		USphereComponent *SignCoreComponent;

	UPROPERTY(EditAnywhere, Category = Movement)
		UProjectileMovementComponent *MovementComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Checks if the actor can be fired and if so it executes the "fire" logic
	 * @param Direction - The difference between the cursor location and the player (as vector)
	 */
	UFUNCTION(BlueprintCallable, Category = Movement)
		void TryFire(FVector Direction);

	/**
	 * Tries to pull back the sign to rotating state.
	 * Called on player input.
	 */
	UFUNCTION(BlueprintCallable, Category = Movement)
		void TryPullBack();

	UPROPERTY(Replicated, BlueprintReadOnly)
	APlayerCharacter* PlayerCharacterRef;

	bool IsReturning();

	FName Team;
};
