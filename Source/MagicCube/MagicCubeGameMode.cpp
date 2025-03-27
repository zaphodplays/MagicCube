// Copyright Epic Games, Inc. All Rights Reserved.

#include "MagicCubeGameMode.h"
#include "MagicCubeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMagicCubeGameMode::AMagicCubeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
