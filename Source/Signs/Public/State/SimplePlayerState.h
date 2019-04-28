// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SimplePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SIGNS_API ASimplePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void AddHit();

	UPROPERTY(BlueprintReadOnly, Replicated)
		int32 Hits = 0;

};
