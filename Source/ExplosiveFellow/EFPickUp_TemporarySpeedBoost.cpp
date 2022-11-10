// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_TemporarySpeedBoost.h"


AEFPickUp_TemporarySpeedBoost::AEFPickUp_TemporarySpeedBoost()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_SpeedBoost"));;
	if (EffectClassFinder.Succeeded())
	{
		EffectClass = EffectClassFinder.Class;
	}
	EffectAmountTagName = FName("EffectMagnitude.SpeedMultiplier");
	Amount = 1.4f;
}
