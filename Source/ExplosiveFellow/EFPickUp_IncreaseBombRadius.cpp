// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_IncreaseBombRadius.h"

AEFPickUp_IncreaseBombRadius::AEFPickUp_IncreaseBombRadius()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_IncreaseRadiusLevelEffect"));;
	if (EffectClassFinder.Succeeded())
	{
		EffectClass = EffectClassFinder.Class;
	}
	EffectAmountTagName = FName("EffectMagnitude.DamageIncrease");
	Amount = 1;
}
