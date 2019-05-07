// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SimplePlayerController.h"
#include "SignsGameInstance.generated.h"

/**
 * The GameInstance persist throghout the levels.
 * Which is why the command functions are executed here.
 * This class is the entry point of client/server travel, 
 * and player spawning
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

	UFUNCTION(Exec)
		void ChooseTeam(const FName& Team);

	UFUNCTION(Exec)
		void GetTeam();
};
