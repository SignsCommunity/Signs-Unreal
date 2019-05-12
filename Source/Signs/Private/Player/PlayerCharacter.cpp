// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Signs.h"
#include "SimplePlayerController.h"
#include "SimpleGameState.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
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

	SpawnSign();
}

/* [Server] */
void APlayerCharacter::SpawnSign()
{
	if (Role == ROLE_Authority) {

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		MainSignRef = GetWorld()->SpawnActor<AMainSign>(MainSignBPRef, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
		if (MainSignRef) {
			MainSignRef->PlayerCharacterRef = this;
		}

	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, MainSignRef);
}

/* [Server] Calls the GameState to add the hit to the score. */
void APlayerCharacter::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other != this && Other->IsA(AMainSign::StaticClass())) {

		AMainSign *hittingSign = Cast<AMainSign>(Other);

		if (hittingSign->IsReturning() || hittingSign->PlayerCharacterRef == this) {
			return;
		}
		UE_LOG(LogSigns, Warning, TEXT("Sign's Team = %s"), *hittingSign->Team.ToString());

		if (Role == ROLE_Authority) {

			hittingSign->IncreaseHotStreak();
			ASimpleGameState* GameState = Cast<ASimpleGameState>(UGameplayStatics::GetGameState(GetWorld()));
			if (GameState) {
				UE_LOG(LogSigns, Warning, TEXT("GameState AddHit Called"));
				GameState->AddHit(hittingSign->Team);
			}

			EnablePhysics();
			GetCapsuleComponent()->AddImpulseAtLocation(OtherComp->ComponentVelocity * 1000, Hit.ImpactPoint);
			GetWorldTimerManager().SetTimer(FiredTimerHandle, this, &APlayerCharacter::DisablePhysics, BounceTime, false);
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
