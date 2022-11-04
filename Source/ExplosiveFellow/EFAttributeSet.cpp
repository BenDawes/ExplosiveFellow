// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAttributeSet.h"
#include "Net/UnrealNetwork.h"


UEFAttributeSet::UEFAttributeSet()
{
}

void UEFAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEFAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UEFAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEFAttributeSet, Health, OldHealth);
}

void UEFAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEFAttributeSet, Stamina, OldStamina);
}
