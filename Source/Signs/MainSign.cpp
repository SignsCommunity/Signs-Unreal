// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSign.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AMainSign::AMainSign() {
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    InitVariables();

	State = EStateEnum::ROTATING;

	InitComponents();
}

// Called when the game starts or when spawned
void AMainSign::BeginPlay() {
    Super::BeginPlay();

}

// Called every frame
void AMainSign::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (State == EStateEnum::ROTATING){
		ContinueOrbitalPath(DeltaTime);
    }

    if (State == EStateEnum::RETURNING){
    	UpdateReturnPath(DeltaTime);
    }
}

void AMainSign::TryFire(FVector Direction){

    if (State == EStateEnum::ROTATING){

    	State = EStateEnum::FIRED;

    	//Velocity fire logic
    	Direction.Z = 0;
    	Direction.Normalize();
    	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
    	GetWorldTimerManager().SetTimer(FiredTimerHandle, this, &AMainSign::ChangeState, MaxFiredTime, false);
    }

}

void AMainSign::ChangeState(){

    if (State == EStateEnum::FIRED){
        State = EStateEnum::RETURNING;
    }
    else if (State == EStateEnum::RETURNING){

		FVector BaseVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
    	RunningTime = 0;
		StartAngle = FMath::Atan2(BaseVector.Y, BaseVector.X) + PI;
		MovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
		CurrentReturnSpeedRatio = SpeedRatioTopBoundary;

        State = EStateEnum::ROTATING;
    }

}

void AMainSign::UpdateReturnPath(float DeltaTime) {

	FVector BaseVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	BaseVector.Z = 0;
	float BaseVectorMagnitude = BaseVector.Size();

	float TangentVectorMagnitude = FMath::Sqrt(FMath::Square(BaseVectorMagnitude) - FMath::Square(OrbitalRadius));

	//finds the tangent from the current position to the orbit
	float OutX = ( TangentVectorMagnitude * BaseVector.X + OrbitalRadius * BaseVector.Y ) / BaseVectorMagnitude;
	float OutY = ( TangentVectorMagnitude * BaseVector.Y - OrbitalRadius * BaseVector.X ) / BaseVectorMagnitude;
	float OutZ = 0;

	FVector OutVector = FVector(OutX, OutY, OutZ);

	if (FMath::IsNearlyZero(OutVector.X, DecreaseSpeedRadius) && FMath::IsNearlyZero(OutVector.Y, DecreaseSpeedRadius)){

		if (CurrentReturnSpeedRatio > SpeedRatioDownBoundary){
			CurrentReturnSpeedRatio -= (DeltaTime * DeltaTimeMultiplier);
		}
	}

	if (FMath::IsNearlyZero(OutVector.X, PullRadius) && FMath::IsNearlyZero(OutVector.Y, PullRadius)){
		ChangeState();
		return;
	}

	OutVector.Normalize();
	MovementComponent->Velocity = OutVector * MovementComponent->InitialSpeed * CurrentReturnSpeedRatio;
}

void AMainSign::ContinueOrbitalPath(float DeltaTime) {

	FVector NewLocation = GetActorLocation();

	FVector CharacterLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//Generate the new location of the pawn relative to the Character Location
	float CurrentAngle = StartAngle + RunningTime * AngularVelocity;

	NewLocation.X = CharacterLocation.X + FMath::Cos(CurrentAngle) * OrbitalRadius;
	NewLocation.Y = CharacterLocation.Y + FMath::Sin(CurrentAngle) * OrbitalRadius;
	NewLocation.Z = CharacterLocation.Z + RelativeHeight;
	RunningTime += DeltaTime;

	SetActorLocation(NewLocation);
}

void AMainSign::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other != this && OtherComp->IsSimulatingPhysics()) {
		OtherComp->AddImpulseAtLocation(MovementComponent->Velocity * ImpulseMultiplier, Hit.ImpactPoint);
	}
}


void AMainSign::InitComponents() {
	// Our root component will be a sphere that reacts to physics
	SignCoreComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SignCoreComponent;
	SignCoreComponent->InitSphereRadius(SignCoreInnerRadius);
	SignCoreComponent->SetNotifyRigidBodyCollision(true);
	SignCoreComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

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
	MovementComponent->SetUpdatedComponent(RootComponent);
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->MaxSpeed = 1000.0f;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 0.5f;
}

void AMainSign::InitVariables(){

	OrbitalRadius = 100.0f;
	AngularVelocity = 3.0f;
	RelativeHeight = 1.0f;
	StartAngle = 0.0f;
	ImpulseMultiplier = 1000.0f;
	PullRadius = 50.0f;
	SpeedRatioTopBoundary = 2.0f;
	SignCoreInnerRadius = 10.0f;
	MaxFiredTime = 2;
	SpeedRatioDownBoundary = 0.4f;
	CurrentReturnSpeedRatio = SpeedRatioTopBoundary;
	DecreaseSpeedRadius = PullRadius * 5;
	DeltaTimeMultiplier = 4;
}
