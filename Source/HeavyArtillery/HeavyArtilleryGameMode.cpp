// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeavyArtilleryGameMode.h"
#include "HeavyArtilleryCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHeavyArtilleryGameMode::AHeavyArtilleryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
