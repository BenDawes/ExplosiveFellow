// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFPickUpCPP.h"
#include "EFPickUp_IncreaseMaxBombs.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API AEFPickUp_IncreaseMaxBombs : public AEFPickUpCPP
{
	GENERATED_BODY()
public:
	AEFPickUp_IncreaseMaxBombs();
	virtual void OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter) override;

};
