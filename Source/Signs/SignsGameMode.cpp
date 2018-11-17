// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SignsGameMode.h"
#include "SignsPlayerController.h"
#include "SignsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASignsGameMode::ASignsGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASignsPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}