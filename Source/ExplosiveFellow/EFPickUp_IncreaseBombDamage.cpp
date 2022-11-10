// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_IncreaseBombDamage.h"

AEFPickUp_IncreaseBombDamage::AEFPickUp_IncreaseBombDamage()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_IncreaseDamageLevelEffect"));;
	if (EffectClassFinder.Succeeded())
	{
		EffectClass = EffectClassFinder.Class;
	}
	EffectAmountTagName = FName("EffectMagnitude.DamageIncrease");
	Amount = 1;
}
