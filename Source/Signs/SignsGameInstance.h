// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SignsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIGNS_API USignsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	USignsGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();
	
	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& Address);
};
