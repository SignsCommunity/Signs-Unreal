// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerProxy.h"
#include "UObject/ConstructorHelpers.h"
#include "SimplePlayerController.h"
#include "UnrealNetwork.h"
#include "Signs.h"

// Sets default values
APlayerProxy::APlayerProxy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	static ConstructorHelpers::FObjectFinder<UClass> PlayerCharacterBP(TEXT("Blueprint'/Game/Player/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerCharacterBP.Object) {
		BP_PlayerCharacterRef = (UClass*)PlayerCharacterBP.Object;
	}
}


// Called every frame
void APlayerProxy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacterRef)
	{
		// Keep the Proxy in sync with the real character
		FTransform PlayerTransform = PlayerCharacterRef->GetTransform();
		FTransform ProxyTransform = GetTransform();

		FTransform Transform;
		Transform.LerpTranslationScale3D(PlayerTransform, ProxyTransform, ScalarRegister(0.5f));

		SetActorTransform(Transform);
	}
}

/* [Server] Spawns PlayerCharacter and the AI Controller */
void APlayerProxy::SpawnPlayer(const FVector &Location)
{
	UE_LOG(LogSigns, Warning, TEXT("Spawning Player"));

	if (Role == ROLE_Authority) {

		FRotator Rotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;


		AIControllerRef = GetWorld()->SpawnActor<AAIController>(Location, Rotation, SpawnParams);
		PlayerCharacterRef = GetWorld()->SpawnActor<APlayerCharacter>(BP_PlayerCharacterRef, Location, Rotation, SpawnParams);


		if (AIControllerRef) {
			AIControllerRef->Possess(PlayerCharacterRef);
		}
	}
}

/* [Server] */
void APlayerProxy::MoveToLocation(const FVector &Destination)
{
	if (AIControllerRef) {
		AIControllerRef->MoveToLocation(Destination);
	}
}

void APlayerProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerProxy, PlayerCharacterRef);
}
