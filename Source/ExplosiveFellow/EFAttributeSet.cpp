// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "EFCustomHealthReactionInterface.h"


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

void UEFAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

	AActor* TargetActor = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	}
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (IEFCustomHealthReactionInterface* AsCustomHealthReactionInterface = Cast<IEFCustomHealthReactionInterface>(TargetActor))
		{
			AsCustomHealthReactionInterface->OnHealthChange(GetHealth());
		}
		else 
		{
			UE_LOG(LogTemp, Log, TEXT("Didn't implement the custom health reaction"));
			// Default health change behaviour
			if (GetHealth() < 0.01f)
			{
				TargetActor->Destroy();
			}
		}
	}
}
