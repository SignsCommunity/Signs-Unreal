// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "MainSign.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SIGNS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Sign")
	class AMainSign* MainSignRef;

	TSubclassOf<class AMainSign> MainSignBPRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
		AController* ClientController;

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

	template<class T>
	T* GetPlayerState() const { return this->ClientController ? this->ClientController->GetPlayerState<T>() : nullptr; }

	UFUNCTION(Exec)
		void SetMyPlayerName(FString name);
	
private:

	/* Enables all physics properties of the CapsuleComponent */
	void EnablePhysics();
	
	/* Disables all physics properties of the CapsuleComponent */
	void DisablePhysics();

	UPROPERTY(EditAnywhere)
	float BounceTime = 2.0f;

	FTimerHandle FiredTimerHandle;

};
