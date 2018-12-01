// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSign.h"

void AMainSign::InitSignCoreAndTrail() {
	// Our root component will be a sphere that reacts to physics
	USphereComponent* SignCoreComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SignCoreComponent;
	SignCoreComponent->InitSphereRadius(SignCoreInnerRadius);
	SignCoreComponent->SetCollisionProfileName(TEXT("SignCore"));

	//// Create and position a mesh component so we can see where our sphere is
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
}

// Sets default values
AMainSign::AMainSign() {
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	OrbitalRadius = 100.0f;
	AngularVelocity = 3.0f;
	RelativeHeight = 1.0f;
	StartAngle = 0.0f;

	SignCoreInnerRadius = 10.0f;

	InitSignCoreAndTrail();
}

// Called when the game starts or when spawned
void AMainSign::BeginPlay() {
    Super::BeginPlay();

	//RootComponent->GetChildComponent(0)->SetWorldScale3D(FVector(SignCoreInnerRadius / 50.0f));
}

// Called every frame
void AMainSign::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();

    FVector CharacterLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    //Generate the new location of the pawn relative to the Character Location
    NewLocation.X = CharacterLocation.X + FMath::Cos(StartAngle + AngularVelocity * RunningTime) * OrbitalRadius;
    NewLocation.Y = CharacterLocation.Y + FMath::Sin(StartAngle + AngularVelocity * RunningTime) * OrbitalRadius;
    NewLocation.Z = CharacterLocation.Z + RelativeHeight;
    RunningTime += DeltaTime;

    SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void AMainSign::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

