// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EFGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "GameplayAbilitySpec.h"
#include "EFGAPlaceBomb.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API UEFGAPlaceBomb : public UEFGameplayAbility
{
	GENERATED_BODY()
	
public:
	UEFGAPlaceBomb();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
