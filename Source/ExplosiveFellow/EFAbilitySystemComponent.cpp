// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAbilitySystemComponent.h"
#include "EFGameplayAbility.h"
#include "EFGAPlaceBomb.h"
#include "EFAttributeSet.h"

#include "Net/UnrealNetwork.h"

UEFAbilitySystemComponent::UEFAbilitySystemComponent()
{
	static ConstructorHelpers::FClassFinder<UEFGameplayAbility> NextBombPenetratesClassFinder(TEXT("/Game/TopDownCPP/GAS/GA_NextBombPenetrates"));;
	if (NextBombPenetratesClassFinder.Succeeded())
	{
		NextBombPenetratesClass = NextBombPenetratesClassFinder.Class;
	}
}

void UEFAbilitySystemComponent::BeginPlay()
{
	UEFAttributeSet* Attributes = (UEFAttributeSet*)GetAttributeSubobject(UEFAttributeSet::StaticClass());
	GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxSpeedAttribute()).AddUObject(this, &UEFAbilitySystemComponent::MaxSpeedChanged);
}


void UEFAbilitySystemComponent::MaxSpeedChanged(const FOnAttributeChangeData& Data)
{
	GenerateBuffString();
}

UEFGAPlaceBomb* UEFAbilitySystemComponent::GetPlaceBombAbility()
{
	auto Abilities = GetActivatableAbilities();
	if (Abilities.Num() == 0)
	{
		return nullptr;
	}

	auto PlaceBombAbility = Abilities.FindByPredicate([](FGameplayAbilitySpec Spec) -> bool { return Spec.Ability->GetClass() == UEFGAPlaceBomb::StaticClass(); });
	if (PlaceBombAbility != nullptr)
	{
		auto ThisASC = this;
		auto Instances = PlaceBombAbility->GetAbilityInstances();

		for (auto Instance : Instances)
		{
			if (Instance->GetAbilitySystemComponentFromActorInfo() == this)
			{
				return (UEFGAPlaceBomb*)Instance;
			}
		}
	}
	return nullptr;
}

void UEFAbilitySystemComponent::SetAbilitiesHaveBeenInitialized(bool NewValue)
{
	bAbilitiesHaveBeenInitialized = NewValue;
}

void UEFAbilitySystemComponent::SetAbilitiesHaveBeenBound(bool NewValue)
{
	bAbilitiesHaveBeenBound = NewValue;
}

void UEFAbilitySystemComponent::GenerateBuffString()
{
	TArray<FString> BuffStrings;
	TSubclassOf<UEFGameplayAbility> LocalNextBombPenetratesClass = NextBombPenetratesClass;
	auto Abilities = GetActivatableAbilities();
	FGameplayAbilitySpec* NextBombPenetratesSpec = Abilities.FindByPredicate([LocalNextBombPenetratesClass](FGameplayAbilitySpec Spec) -> bool { return Spec.Ability->GetClass() == LocalNextBombPenetratesClass; });
	if (NextBombPenetratesSpec != nullptr)
	{
		auto NextBombPenetratesHandle = NextBombPenetratesSpec->Handle;
		auto IsPendingRemoval = AbilityPendingRemoves.ContainsByPredicate([NextBombPenetratesHandle](FGameplayAbilitySpecHandle PendingRemoval) ->
			bool { return PendingRemoval == NextBombPenetratesHandle; });

		if (!IsPendingRemoval) {
			BuffStrings.Add(TEXT("Piercing bomb"));
		}
	}
	UEFAttributeSet* Attributes = (UEFAttributeSet*)GetAttributeSubobject(UEFAttributeSet::StaticClass());
	if (Attributes != nullptr)
	{
		auto Data = Attributes->GetMaxSpeedAttribute().GetGameplayAttributeData(Attributes);
		if (Data->GetBaseValue() != Data->GetCurrentValue())
		{
			BuffStrings.Add(TEXT("Speed boost"));
		}
	}
	BuffString = FString::Join(BuffStrings, TEXT(", "));
}

void UEFAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFAbilitySystemComponent, BuffString);
}