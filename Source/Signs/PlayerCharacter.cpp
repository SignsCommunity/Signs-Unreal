// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "UnrealNetwork.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	static ConstructorHelpers::FObjectFinder<UClass> MainSignBlueprint(TEXT("Blueprint'/Game/TopDownCPP/Blueprints/BP_Sign.BP_Sign_C'"));
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
			//TODO Logic for adding force/impulse on character
		}
	}
}
