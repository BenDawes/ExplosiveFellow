// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_Heal.h"

AEFPickUp_Heal::AEFPickUp_Heal()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_HealEffect"));;
	if (EffectClassFinder.Succeeded())
	{
		EffectClass = EffectClassFinder.Class;
	}
	EffectAmountTagName = FName("EffectMagnitude.Heal");
	Amount = .25f;
	TextureAsset = FSoftObjectPath(TEXT("Texture2D'/Game/TopDownCPP/Sprites/Potions/Half-Red.Half-Red'"));
}