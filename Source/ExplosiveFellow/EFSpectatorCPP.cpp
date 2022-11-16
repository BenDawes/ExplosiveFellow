// Fill out your copyright notice in the Description page of Project Settings.


#include "EFSpectatorCPP.h"

// Sets default values
AEFSpectatorCPP::AEFSpectatorCPP()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

// Called when the game starts or when spawned
void AEFSpectatorCPP::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* Controller = Cast<APlayerController>(GetController());
	if (Controller != nullptr)
	{
		Controller->bShowMouseCursor = true;
	}
}

// Called every frame
void AEFSpectatorCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEFSpectatorCPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

