// Fill out your copyright notice in the Description page of Project Settings.

#include "SignsGameInstance.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "SimplePlayerController.h"
#include "Signs.h"
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


	World->ServerTravel("/Game/Levels/Arena/Maps/ArenaMap?listen");
}


void USignsGameInstance::Join(const FString& Address)
{

	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* LocalPlayer = GetFirstLocalPlayerController();
	if (!ensure(LocalPlayer != nullptr)) return;

	LocalPlayer->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


/* Assigns a team and calls method to spawn player on the Server. */
void USignsGameInstance::ChooseTeam(const FName& Team)
{

	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Choosing Team: ")));

	APlayerController* LocalPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ensure(LocalPC != nullptr)) return;

	ASimplePlayerController *SimpleLocalPC = Cast<ASimplePlayerController>(LocalPC);
	if (!ensure(SimpleLocalPC != nullptr)) return;


	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Spawning !!! ")));

	SimpleLocalPC->ServerSpawnPlayer(Team);
	SimpleLocalPC->Team = Team;
}

void USignsGameInstance::GetTeam()
{
	APlayerController* LocalPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ensure(LocalPC != nullptr)) return;

	ASimplePlayerController *SimpleLocalPC = Cast<ASimplePlayerController>(LocalPC);
	if (!ensure(SimpleLocalPC != nullptr)) return;

	UE_LOG(LogSigns, Warning, TEXT("My Team: %s"), *SimpleLocalPC->Team.ToString());
}
