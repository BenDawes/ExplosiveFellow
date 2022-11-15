// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowGameMode.h"
#include "EFGameState.h"
#include "EFPickUpProvider.h"
#include "ExplosiveFellowPlayerController.h"
#include "ExplosiveFellowCharacter.h"
#include "Templates/SharedPointer.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include <memory>
#include "HAL/PlatformApplicationMisc.h"

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

	GameStateClass = AEFGameState::StaticClass();

	static ConstructorHelpers::FObjectFinder<UDataTable> PickUpDataTableFinder(TEXT("DataTable'/Game/TopDownCPP/Blueprints/PickUpSpawnTable.PickUpSpawnTable'"));
	if (PickUpDataTableFinder.Succeeded())
	{
		PickUpDataTable = PickUpDataTableFinder.Object;
	}
}


void AExplosiveFellowGameMode::BeginPlay()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PickUpProvider = GetWorld()->SpawnActor<AEFPickUpProvider>(AEFPickUpProvider::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	PickUpProvider->GenerateFromDataTable(PickUpDataTable);
}

void AExplosiveFellowGameMode::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer(NewPlayer);
}

AEFPickUpProvider* AExplosiveFellowGameMode::GetPickUpProvider()
{
	return PickUpProvider;
}

void AExplosiveFellowGameMode::CopyToClipboard(FString StrToCopy)
{
	FPlatformApplicationMisc::ClipboardCopy(*StrToCopy);
}
