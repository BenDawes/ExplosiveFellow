// Fill out your copyright notice in the Description page of Project Settings.


#include "EFGAPlaceBomb.h"
#include "EFBomb.h"


UEFGAPlaceBomb::UEFGAPlaceBomb() {}

void UEFGAPlaceBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FVector SpawnLocation = TriggerEventData->Target->GetTargetLocation();
	FRotator SpawnRotation = TriggerEventData->Instigator->GetActorRotation();
	
	UWorld* World = GetWorld();
	if (World == nullptr) {
		return;
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<AEFBomb>(AEFBomb::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
}
