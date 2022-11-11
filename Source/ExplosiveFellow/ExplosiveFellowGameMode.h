// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Memory>
#include "GameFramework/GameModeBase.h"
#include "EFPickUpProvider.h"
#include "ExplosiveFellowGameMode.generated.h"

UCLASS(minimalapi)
class AExplosiveFellowGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AExplosiveFellowGameMode();
	virtual void BeginPlay() override;

	void HandleStartingNewPlayer(APlayerController* NewPlayer);

	AEFPickUpProvider* GetPickUpProvider();

protected:
	AEFPickUpProvider* PickUpProvider;
};

