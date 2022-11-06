// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExplosiveFellowPlayerController.generated.h"

UCLASS()
class AExplosiveFellowPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AExplosiveFellowPlayerController();
	virtual void AcknowledgePossession(APawn* NewPawn) override;

};


