// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowGameMode.h"
#include "ExplosiveFellowPlayerController.h"
#include "ExplosiveFellowCharacter.h"
#include "UObject/ConstructorHelpers.h"

AExplosiveFellowGameMode::AExplosiveFellowGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AExplosiveFellowPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Spectator"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AExplosiveFellowGameMode::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer(NewPlayer);
}