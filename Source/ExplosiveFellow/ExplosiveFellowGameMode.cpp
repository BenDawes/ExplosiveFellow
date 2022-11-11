// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowGameMode.h"
#include "EFPickUpProvider.h"
#include "ExplosiveFellowPlayerController.h"
#include "ExplosiveFellowCharacter.h"
#include "Templates/SharedPointer.h"
#include "UObject/ConstructorHelpers.h"
#include <memory>

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


void AExplosiveFellowGameMode::BeginPlay()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PickUpProvider = GetWorld()->SpawnActor<AEFPickUpProvider>(AEFPickUpProvider::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
}

void AExplosiveFellowGameMode::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer(NewPlayer);
}

AEFPickUpProvider* AExplosiveFellowGameMode::GetPickUpProvider()
{
	return PickUpProvider;
}