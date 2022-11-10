// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUp_NextBombPenetrates.h"
#include "EFGameplayAbility.h"
#include "EFAbilitySystemComponent.h"

AEFPickUp_NextBombPenetrates::AEFPickUp_NextBombPenetrates()
{
	static ConstructorHelpers::FClassFinder<UEFGameplayAbility> AbilityFinder(TEXT("/Game/TopDownCPP/GAS/GA_NextBombPenetrates"));;
	if (AbilityFinder.Succeeded())
	{
		NextBombPenetratesAbility = AbilityFinder.Class;
	}
}

void AEFPickUp_NextBombPenetrates::OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter)
{
	if (HasAuthority())
	{
		ReceivingCharacter->GetAbilitySystemComponent()->GiveAbility(
			FGameplayAbilitySpec(NextBombPenetratesAbility, 1, -1, this)
		);
		Destroy();
	}
}