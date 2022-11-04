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
};
