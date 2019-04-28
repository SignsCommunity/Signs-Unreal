// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Signs.h"
#include "SimplePlayerState.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	static ConstructorHelpers::FObjectFinder<UClass> MainSignBlueprint(TEXT("Blueprint'/Game/Sign/BP_Sign.BP_Sign_C'"));
	if (MainSignBlueprint.Object) {
		MainSignBPRef = (UClass*)MainSignBlueprint.Object;
	}
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		MainSignRef = GetWorld()->SpawnActor<AMainSign>(MainSignBPRef, FVector(0, 0, 0), FRotator(0, 0, 0));
		if (MainSignRef) {
			MainSignRef->PlayerCharacterRef = this;
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, MainSignRef);
}

void APlayerCharacter::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other != this && Other->IsA(AMainSign::StaticClass())) {
		if (HasAuthority()) {
			EnablePhysics();
			GetCapsuleComponent()->AddImpulseAtLocation(OtherComp->ComponentVelocity * 1000, Hit.ImpactPoint);
			GetWorldTimerManager().SetTimer(FiredTimerHandle, this, &APlayerCharacter::DisablePhysics, BounceTime, false);

			UE_LOG(LogSigns, Warning, TEXT("NotifyHit"));

			if (this->ClientController == nullptr)
			{
				UE_LOG(LogSigns, Warning, TEXT("ClientController null"));
				return;
			}
			 
			if (this->ClientController->PlayerState == nullptr) 
				UE_LOG(LogSigns, Warning, TEXT("Playerstate null"));
			
			ASimplePlayerState* playerState = Cast<ASimplePlayerState>(this->ClientController->PlayerState);
			if (playerState && Other != this->MainSignRef) {
				UE_LOG(LogSigns, Warning, TEXT("Cast successful"));
				playerState->AddHit();
			}
		}
	}
}
 
void APlayerCharacter::DisablePhysics()
{
	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetCollisionProfileName("CharacterMesh");
}

void APlayerCharacter::EnablePhysics()
{
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
}

void APlayerCharacter::SetMyPlayerName(FString name)
{
	if(ASimplePlayerState* playerState = GetPlayerState<ASimplePlayerState>())
		playerState->SetPlayerName(name);
}