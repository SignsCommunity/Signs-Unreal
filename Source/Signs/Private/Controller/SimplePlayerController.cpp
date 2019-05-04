// Fill out your copyright notice in the Description page of Project Settings.

#include "SimplePlayerController.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "SimpleGameState.h"
#include "SimpleGameMode.h"
#include "PlayerProxy.h"
#include "Signs.h"

#define GET_CONTROLLED_PAWN if (!GetPawn()) {return;}; APlayerProxy* ControlledPawn = Cast<APlayerProxy>(GetPawn()); if (!ControlledPawn) { return; };

// Called to bind functionality to input
void ASimplePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ASimplePlayerController::MoveToLocation);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ASimplePlayerController::TryFire);
	InputComponent->BindAction("Pull", IE_Pressed, this, &ASimplePlayerController::TryPull);
}

/* [Server] */
bool ASimplePlayerController::ServerTryFire_Validate(const FVector &Direction) { return true; }
void ASimplePlayerController::ServerTryFire_Implementation(const FVector &Direction)
{
	GET_CONTROLLED_PAWN
		ControlledPawn->PlayerCharacterRef->MainSignRef->TryFire(Direction);
}

/* [Server] */
bool ASimplePlayerController::ServerTryPull_Validate() { return true; }
void ASimplePlayerController::ServerTryPull_Implementation()
{
	GET_CONTROLLED_PAWN
		ControlledPawn->PlayerCharacterRef->MainSignRef->TryPullBack();
}

/* [Server] */
bool ASimplePlayerController::ServerMoveToLocation_Validate(const FVector &HitResultLocation) { return true; }
void ASimplePlayerController::ServerMoveToLocation_Implementation(const FVector &HitResultLocation)
{
	GET_CONTROLLED_PAWN
		ControlledPawn->MoveToLocation(HitResultLocation);
}

/* [Client] */
void ASimplePlayerController::TryFire()
{
	if (Role < ROLE_Authority) {
		GET_CONTROLLED_PAWN
			ServerTryFire(GetMouseRayHitLocation() - ControlledPawn->GetActorLocation());
	}
	else {
		GET_CONTROLLED_PAWN
			ControlledPawn->PlayerCharacterRef->MainSignRef->TryFire(GetMouseRayHitLocation() - ControlledPawn->GetActorLocation());
	}
}

/* [Client] */
void ASimplePlayerController::TryPull()
{
	if (Role < ROLE_Authority) {
		ServerTryPull();
	}
	else {
		GET_CONTROLLED_PAWN
			ControlledPawn->PlayerCharacterRef->MainSignRef->TryPullBack();
	}
}

/* [Client] */
void ASimplePlayerController::MoveToLocation()
{
	if (GetMouseRayHitLocation() != FVector::ZeroVector) {
		//Found legit hit location
		ServerMoveToLocation(GetMouseRayHitLocation());
	}
}

/* [Client] */
FVector ASimplePlayerController::GetMouseRayHitLocation()
{
	GetMousePosition(LocationX, LocationY);
	FVector2D MousePos(LocationX, LocationY);

	FHitResult HitResult;
	const bool bTraceComplex = false;

	if (GetHitResultAtScreenPosition(MousePos, ECC_Visibility, bTraceComplex, HitResult) == true)
	{
		return HitResult.Location;
	}
	return FVector::ZeroVector;
}


/* [Server] */
bool ASimplePlayerController::ServerSpawnPlayer_Validate(const FName& Team) { return true; }
void ASimplePlayerController::ServerSpawnPlayer_Implementation(const FName& Team)
{
	ASimpleGameMode *GM = Cast<ASimpleGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {
		FVector SpawnPosition = GM->GetPlayerSpawnPosition(this, Team);

		if (SpawnPosition == FVector::ZeroVector) {
			//Player choose non-existent team
			return;
		}

		GET_CONTROLLED_PAWN
			ControlledPawn->SpawnPlayer(SpawnPosition);

		ControlledPawn->PlayerCharacterRef->MainSignRef->Team = Team;

		UE_LOG(LogSigns, Warning, TEXT("GM CAST SUCCESS %f"), SpawnPosition.Z);
	}
}
