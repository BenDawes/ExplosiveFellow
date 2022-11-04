// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include <ExplosiveFellow/ExplosiveFellow.h>
#include "EFGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API UEFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEFGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
