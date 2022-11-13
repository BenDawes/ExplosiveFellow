// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_TemporarySpeedBoost.h"
#include "EFAbilitySystemComponent.h"

AEFPickUp_TemporarySpeedBoost::AEFPickUp_TemporarySpeedBoost()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> EffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_SpeedBoost"));;
	if (EffectClassFinder.Succeeded())
	{
		EffectClass = EffectClassFinder.Class;
	}
	EffectAmountTagName = FName("EffectMagnitude.SpeedMultiplier");
	Amount = 10.f;
}


void AEFPickUp_TemporarySpeedBoost::OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter)
{
	ApplyStandardEffect(ReceivingCharacter);
	auto ASC = Cast<UEFAbilitySystemComponent>(ReceivingCharacter->GetAbilitySystemComponent());
	if (ASC != nullptr)
	{
		ASC->GenerateBuffString();
	}
	Destroy();
}