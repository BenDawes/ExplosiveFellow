// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFPickUpCPP.h"
#include "EFPickUpProvider.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API AEFPickUpProvider : public AActor
{
	GENERATED_BODY()
public:
	AEFPickUpProvider();
	TSubclassOf<AEFPickUpCPP> GetRandomPickUpClass();
	TArray<TSubclassOf<AEFPickUpCPP>> PickupClasses;
};
