// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EFGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "EFGAPlaceBomb.h"
#include "EFAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API UEFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UEFAbilitySystemComponent();
	virtual void BeginPlay() override;
	void SetAbilitiesHaveBeenInitialized(bool NewValue);
	bool AbilitiesHaveBeenInitialized() { return bAbilitiesHaveBeenInitialized; }
	void SetAbilitiesHaveBeenBound(bool NewValue);
	bool AbilitiesHaveBeenBound() { return bAbilitiesHaveBeenBound; }
	void GenerateBuffString();

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	FString BuffString;

	void MaxSpeedChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	UEFGAPlaceBomb* GetPlaceBombAbility();
private:
	bool bAbilitiesHaveBeenInitialized = false;
	bool bAbilitiesHaveBeenBound = false;
	TSubclassOf<UEFGameplayAbility> NextBombPenetratesClass;
};
