// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAbilitySystemComponent.h"

void UEFAbilitySystemComponent::SetAbilitiesHaveBeenInitialized(bool NewValue)
{
	bAbilitiesHaveBeenInitialized = NewValue;
}

void UEFAbilitySystemComponent::SetAbilitiesHaveBeenBound(bool NewValue)
{
	bAbilitiesHaveBeenBound = NewValue;
}