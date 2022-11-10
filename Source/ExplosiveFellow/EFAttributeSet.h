// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EFAttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class EXPLOSIVEFELLOW_API UEFAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UEFAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEFAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEFAttributeSet, Stamina);
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_BombSizeLevel)
	FGameplayAttributeData BombSizeLevel;
	ATTRIBUTE_ACCESSORS(UEFAttributeSet, BombSizeLevel);
	UFUNCTION()
	virtual void OnRep_BombSizeLevel(const FGameplayAttributeData& OldBombSizeLevel);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_BombDamageLevel)
	FGameplayAttributeData BombDamageLevel;
	ATTRIBUTE_ACCESSORS(UEFAttributeSet, BombDamageLevel);
	UFUNCTION()
	virtual void OnRep_BombDamageLevel(const FGameplayAttributeData& OldBombDamageLevel);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_MaxBombs)
	FGameplayAttributeData MaxBombs;
	ATTRIBUTE_ACCESSORS(UEFAttributeSet, MaxBombs);
	UFUNCTION()
	virtual void OnRep_MaxBombs(const FGameplayAttributeData& OldMaxBombs);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", ReplicatedUsing = OnRep_MaxSpeed)
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UEFAttributeSet, MaxSpeed);
	UFUNCTION()
	virtual void OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
