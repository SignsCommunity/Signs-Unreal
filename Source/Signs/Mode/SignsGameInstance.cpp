// Fill out your copyright notice in the Description page of Project Settings.

#include "SignsGameInstance.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

USignsGameInstance::USignsGameInstance(const FObjectInitializer & ObjectInitializer) 
{

}

void USignsGameInstance::Init()
{
	//Called when game is started. Initial setup should be done here (as opposed to the constructor)
}

void USignsGameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(0, 4, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (World == nullptr) return;


	World->ServerTravel("/Game/TopDownCPP/Maps/TopDownExampleMap?listen");
}


void USignsGameInstance::Join(const FString& Address)
{

	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* LocalPlayer = GetFirstLocalPlayerController();
	if (!ensure(LocalPlayer != nullptr)) return;

	LocalPlayer->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

