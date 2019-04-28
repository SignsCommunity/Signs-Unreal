// Fill out your copyright notice in the Description page of Project Settings.

#include "SimplePlayerState.h"
#include "UnrealNetwork.h"

// debug headers
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Signs.h"
// end debug header

void ASimplePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASimplePlayerState, Hits);
}

void ASimplePlayerState::AddHit() {
	Hits++;
	UE_LOG(LogSigns, Warning, TEXT("Player hit"));
	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, FString::Printf(TEXT("Player: %s hit!"), *PlayerName));
	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Green, FString::Printf(TEXT("Player hits: %d"), this->Hits));
}
