// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSign.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Signs.h"
#include "PlayerCharacter.h"

// Sets default values
AMainSign::AMainSign() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

	State = EStateEnum::ROTATING;
	InitComponents();
}

void AMainSign::BeginPlay() {
	Super::BeginPlay();

	InitVariables();
}


void AMainSign::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainSign, State);
	DOREPLIFETIME(AMainSign, PlayerCharacterRef);
	DOREPLIFETIME(AMainSign, AngularVelocity);
	DOREPLIFETIME(AMainSign, StartAngle);
}

/* [Server] */
void AMainSign::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!PlayerCharacterRef) {
		return;
	}


	if (State == EStateEnum::ROTATING) {
		Slowdown(DeltaTime);
		ContinueOrbitalPath(DeltaTime);
	}

	if (State == EStateEnum::RETURNING) {
		UpdateReturnPath(DeltaTime);
	}

	FVector Location = GetActorLocation();
	Location.Z = PlayerCharacterRef->GetActorLocation().Z + RelativeHeight;
	SetActorLocation(Location);
}

/* [Server] */
void AMainSign::TryFire(FVector Direction) {

	if (State == EStateEnum::ROTATING) {

		State = EStateEnum::FIRED;

		//Velocity fire logic
		Direction.Z = 0;
		Direction.Normalize();
		MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
	}
}

/* [Server] */
void AMainSign::TryPullBack() {
	
	if (State == EStateEnum::FIRED && IsOutsideOrbit()) {
		State = EStateEnum::RETURNING;
	}
}

bool AMainSign::IsReturning() {
	return (State == EStateEnum::RETURNING);
}

bool AMainSign::IsOutsideOrbit() {

	FVector DistanceVector = PlayerCharacterRef->GetActorLocation() - GetActorLocation();
	return (DistanceVector.Size() > OrbitalRadius);
}

void AMainSign::ChangeState() {

	if (State == EStateEnum::FIRED) {
		State = EStateEnum::RETURNING;
	}
	else if (State == EStateEnum::RETURNING) {

		FVector BaseVector = PlayerCharacterRef->GetActorLocation() - GetActorLocation();
		RunningTime = 0;
		StartAngle = FMath::Atan2(BaseVector.Y, BaseVector.X) + PI;
		MovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
		CurrentReturnSpeedRatio = SpeedRatioTopBoundary;
		AngularVelocity = AngularVelocityCap;

		State = EStateEnum::ROTATING;
	}
}

void AMainSign::UpdateReturnPath(float DeltaTime) {

	FVector BaseVector = PlayerCharacterRef->GetActorLocation() - GetActorLocation();
	BaseVector.Z = 0;
	float BaseVectorMagnitude = BaseVector.Size();

	float TangentVectorMagnitude = FMath::Sqrt(FMath::Square(BaseVectorMagnitude) - FMath::Square(OrbitalRadius));

	//finds the tangent from the current position to the orbit
	float OutX = (TangentVectorMagnitude * BaseVector.X + OrbitalRadius * BaseVector.Y) / BaseVectorMagnitude;
	float OutY = (TangentVectorMagnitude * BaseVector.Y - OrbitalRadius * BaseVector.X) / BaseVectorMagnitude;
	float OutZ = 0;

	FVector OutVector = FVector(OutX, OutY, OutZ);

	if (FMath::IsNearlyZero(OutVector.X, DecreaseSpeedRadius) && FMath::IsNearlyZero(OutVector.Y, DecreaseSpeedRadius)) {

		if (CurrentReturnSpeedRatio > SpeedRatioDownBoundary) {
			CurrentReturnSpeedRatio -= (DeltaTime * DeltaTimeMultiplier);
		}
	}

	if (FMath::IsNearlyZero(OutVector.X, PullRadius) && FMath::IsNearlyZero(OutVector.Y, PullRadius)) {
		ChangeState();
		return;
	}

	OutVector.Normalize();
	MovementComponent->Velocity = OutVector * MovementComponent->InitialSpeed * CurrentReturnSpeedRatio;
}

void AMainSign::ContinueOrbitalPath(float DeltaTime) {

	FVector NewLocation = GetActorLocation();

	FVector CharacterLocation = PlayerCharacterRef->GetActorLocation();

	//Generate the new location of the pawn relative to the Character Location
	float CurrentAngle = StartAngle + RunningTime * AngularVelocity;

	NewLocation.X = CharacterLocation.X + FMath::Cos(CurrentAngle) * OrbitalRadius;
	NewLocation.Y = CharacterLocation.Y + FMath::Sin(CurrentAngle) * OrbitalRadius;
	NewLocation.Z = CharacterLocation.Z + RelativeHeight;
	RunningTime += DeltaTime;

	SetActorLocation(NewLocation);
}

void AMainSign::TrySwitch() {
	StartAngle += PI;
}

void AMainSign::IncreaseHotStreak() {
	if (AngularVelocity < AngularVelocityCap) {
		AngularVelocity *= AngularVelocityMultiplier;
	}
}

void AMainSign::Slowdown(float DeltaTime) {
	if (AngularVelocity > StartAngularVelocity) {
		AngularVelocity -= (DeltaTime * (DeltaTimeMultiplier / 2));
	}
}

void AMainSign::InitComponents() {
	// Our root component will be a sphere that reacts to physics
	SignCoreComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SignCoreComponent;
	SignCoreComponent->InitSphereRadius(SignCoreInnerRadius);
	SignCoreComponent->SetNotifyRigidBodyCollision(true);
	SignCoreComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));


	// Create a component that will drive the sign fired movement
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SignMovementComponent"));
	MovementComponent->SetUpdatedComponent(RootComponent);
	MovementComponent->InitialSpeed = ImpulseMultiplier;
	MovementComponent->MaxSpeed = 1000.0f;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 0.5f;
}

void AMainSign::InitVariables() {

	OrbitalRadius = 100.0f;
	StartAngularVelocity = 4.0f;
	AngularVelocityCap = 5.5f;
	RelativeHeight = 1.0f;

	StartAngle = 0.0f;
	PullRadius = 110.0f;
	SpeedRatioTopBoundary = 3.0f;
	SignCoreInnerRadius = 10.0f;
	MaxFiredTime = 2;
	SpeedRatioDownBoundary = 1.4f;
	DecreaseSpeedRadius = 500.0f;
	DeltaTimeMultiplier = 4;

	AngularVelocity = StartAngularVelocity;
	CurrentReturnSpeedRatio = SpeedRatioTopBoundary;
}
