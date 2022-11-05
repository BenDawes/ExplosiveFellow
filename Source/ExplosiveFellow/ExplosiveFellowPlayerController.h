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
	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 700;
	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

};


