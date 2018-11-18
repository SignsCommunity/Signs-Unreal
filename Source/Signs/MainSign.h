// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <EngineClasses.h>
#include "MainSign.generated.h"

UCLASS()
class SIGNS_API AMainSign : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainSign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SignBehaviour")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SignBehaviour")
	float AngularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SignBehaviour")
	float StartAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SignBehaviour")
	float RelativeHeight;
};
