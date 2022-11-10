// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFPickUpCPP.h"
#include "EFPickUp_NextBombPenetrates.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API AEFPickUp_NextBombPenetrates : public AEFPickUpCPP
{
	GENERATED_BODY()
public:
	AEFPickUp_NextBombPenetrates();
	virtual void OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter) override;
	TSubclassOf<class UEFGameplayAbility> NextBombPenetratesAbility;
};
