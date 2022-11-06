// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ExplosiveFellowCharacter.h"
#include "Components/InputComponent.h"
#include "EFAbilitySystemComponent.h"
#include "ExplosiveFellow.h"
#include "Engine/World.h"

AExplosiveFellowPlayerController::AExplosiveFellowPlayerController()
{
}

void AExplosiveFellowPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	AExplosiveFellowCharacter* ControlledCharacter = Cast<AExplosiveFellowCharacter>(GetPawn());
	if (ControlledCharacter == nullptr)
	{
		return;
	}
	auto AbilitySystemComponent = ControlledCharacter->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(ControlledCharacter, ControlledCharacter);
}