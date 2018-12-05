// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSign.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AMainSign::AMainSign() {
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	OrbitalRadius = 100.0f;
	AngularVelocity = 3.0f;
	RelativeHeight = 1.0f;
	StartAngle = 0.0f;

	SignCoreInnerRadius = 10.0f;

	MaxFiredTime = 2;
	State = EStateEnum::ROTATING;

	InitComponents();
}

// Called when the game starts or when spawned
void AMainSign::BeginPlay() {
    Super::BeginPlay();

	//RootComponent->GetChildComponent(0)->SetWorldScale3D(FVector(SignCoreInnerRadius / 50.0f));
}

// Called every frame
void AMainSign::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (State == EStateEnum::ROTATING){
		ContinueOrbitalPath(DeltaTime);
    }

}

// Called to bind functionality to input
void AMainSign::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainSign::TryFire(FVector Direction){

    if (State != EStateEnum::FIRED){

    	State = EStateEnum::FIRED;

    	//Velocity logic
    	Direction.Z = 0;
    	Direction.Normalize();
    	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
    	GetWorldTimerManager().SetTimer(FiredTimerHandle, this, &AMainSign::ReturnToOrbit, MaxFiredTime, false);
    }

}

void AMainSign::ReturnToOrbit(){

	MovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
	State = EStateEnum::ROTATING;
}

void AMainSign::ContinueOrbitalPath(float DeltaTime) {

	FVector NewLocation = GetActorLocation();

	FVector CharacterLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//Generate the new location of the pawn relative to the Character Location
	NewLocation.X = CharacterLocation.X + FMath::Cos(StartAngle + AngularVelocity * RunningTime) * OrbitalRadius;
	NewLocation.Y = CharacterLocation.Y + FMath::Sin(StartAngle + AngularVelocity * RunningTime) * OrbitalRadius;
	NewLocation.Z = CharacterLocation.Z + RelativeHeight;
	RunningTime += DeltaTime;

	SetActorLocation(NewLocation);
}

void AMainSign::InitComponents() {
	// Our root component will be a sphere that reacts to physics
	SignCoreComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SignCoreComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	SignCoreComponent->InitSphereRadius(SignCoreInnerRadius);
	RootComponent = SignCoreComponent;

	// Create and position a mesh component so we can see where our sphere is
	SignCoreVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignCoreVisual"));
	SignCoreVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SignCoreVisual->SetStaticMesh(SphereVisualAsset.Object);
		SignCoreVisual->SetWorldScale3D(FVector(SignCoreInnerRadius / 50.0f));
	}

	// Create a particle system for the Sign's Trail that we can activate or deactivate
	TrailParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticles"));
	TrailParticleSystem->SetupAttachment(SignCoreVisual);
	TrailParticleSystem->bAutoActivate = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Sign/Particles/P_Trail.P_Trail"));
	if (ParticleAsset.Succeeded())
	{
		TrailParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	// Create a component that will drive the sign fired movement
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SignMovementComponent"));
	MovementComponent->SetUpdatedComponent(SignCoreComponent);
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->MaxSpeed = 1000.0f;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->bRotationFollowsVelocity = true;
}
