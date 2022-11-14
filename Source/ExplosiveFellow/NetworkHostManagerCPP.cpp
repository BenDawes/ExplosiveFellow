// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkHostManagerCPP.h"

// Sets default values
ANetworkHostManagerCPP::ANetworkHostManagerCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetworkHostManagerCPP::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANetworkHostManagerCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

