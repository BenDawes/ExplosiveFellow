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
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDClassFinder(TEXT("/Game/TopDownCPP/UI/HUD"));;
	if (HUDClassFinder.Succeeded())
	{
		HUDClass = HUDClassFinder.Class;
	}
}

void AExplosiveFellowPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	AExplosiveFellowCharacter* ControlledCharacter = Cast<AExplosiveFellowCharacter>(GetPawn());
	if (ControlledCharacter == nullptr)
	{
		bShowMouseCursor = true;
		return;
	}
	auto AbilitySystemComponent = ControlledCharacter->GetAbilitySystemComponent();
	bShowMouseCursor = false;

	AbilitySystemComponent->InitAbilityActorInfo(ControlledCharacter, ControlledCharacter);
	ControlledCharacter->SetIsAIControlled(false);
	PlayerUI = CreateWidget(GetWorld(), HUDClass, FName("PlayerHUD"));
	PlayerUI->AddToViewport();
	ControlledCharacter->OnDestroyed.AddDynamic(this, &AExplosiveFellowPlayerController::OnPlayerDestroy);
}

void AExplosiveFellowPlayerController::OnPlayerDestroy(AActor* Actor)
{
	PlayerUI->RemoveFromViewport();
}
