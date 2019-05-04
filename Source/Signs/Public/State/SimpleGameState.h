// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SimpleGameState.generated.h"

/**
 * 
 */
UCLASS()
class SIGNS_API ASimpleGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 TeamAScore;
	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 TeamBScore;

	void AddHit(const FName &Team);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

};
