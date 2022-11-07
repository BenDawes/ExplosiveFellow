// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ExplosiveFellowGameMode.generated.h"

UCLASS(minimalapi)
class AExplosiveFellowGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AExplosiveFellowGameMode();

	// TODO ChoosePlayerStart currently picks a random unoccupied start. Is that sufficient? If not, override.
	
	// void HandleStartingNewPlayer(APlayerController* NewPlayer);
	void HandleStartingNewPlayer(APlayerController* NewPlayer);
};

