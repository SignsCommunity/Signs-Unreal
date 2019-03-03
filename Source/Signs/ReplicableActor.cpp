// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplicableActor.h"

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "MainSign.h"

// Sets default values
AReplicableActor::AReplicableActor()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AReplicableActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AReplicableActor::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other != this && Other->IsA(AMainSign::StaticClass())) {

		if (HasAuthority()) {
			StaticMesh->AddImpulseAtLocation(OtherComp->ComponentVelocity * 1000, Hit.ImpactPoint);
		}
	}
}

