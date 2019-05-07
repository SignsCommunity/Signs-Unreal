// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleGameState.h"
#include "UnrealNetwork.h"
#include "Signs.h"

void ASimpleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASimpleGameState, TeamAScore);
	DOREPLIFETIME(ASimpleGameState, TeamBScore);
}

void ASimpleGameState::AddHit(const FName &Team) {
	if (Team.IsEqual(TEXT("Team_A"))) {
		TeamAScore++;
	}
	else {
		TeamBScore++;
	}

	UE_LOG(LogSigns, Warning, TEXT("Hit in Game State TeamAScore: %d"), TeamAScore);
	UE_LOG(LogSigns, Warning, TEXT("Hit in Game State TeamBScore: %d"), TeamBScore);
}
