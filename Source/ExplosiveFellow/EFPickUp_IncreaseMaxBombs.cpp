// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_IncreaseMaxBombs.h"

AEFPickUp_IncreaseMaxBombs::AEFPickUp_IncreaseMaxBombs()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_IncreaseMaxBombs"));;
	if (EffectClassFinder.Succeeded())
	{
		EffectClass = EffectClassFinder.Class;
	}
}


void AEFPickUp_IncreaseMaxBombs::OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter)
{
	ApplyStandardEffect(ReceivingCharacter, false);
	Destroy();
}