// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicableActor.generated.h"

UCLASS()
class SIGNS_API AReplicableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReplicableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyHit
	(
		class UPrimitiveComponent * MyComp,
		AActor * Other,
		class UPrimitiveComponent * OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult & Hit
	);

private:

	UPROPERTY(EditAnywhere, Category = "Appearance")
		UStaticMeshComponent *StaticMesh;
};
