// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EFAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API UEFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void SetAbilitiesHaveBeenInitialized(bool NewValue);
	bool AbilitiesHaveBeenInitialized() { return bAbilitiesHaveBeenInitialized; }
	void SetAbilitiesHaveBeenBound(bool NewValue);
	bool AbilitiesHaveBeenBound() { return bAbilitiesHaveBeenBound; }
private:
	bool bAbilitiesHaveBeenInitialized = false;
	bool bAbilitiesHaveBeenBound = false;
};
