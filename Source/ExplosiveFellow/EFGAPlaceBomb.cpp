// Fill out your copyright notice in the Description page of Project Settings.


#include "EFGAPlaceBomb.h"
#include "EFBomb.h"


UEFGAPlaceBomb::UEFGAPlaceBomb() {}

void UEFGAPlaceBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("Spawning bomb"));
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
		FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();

		UWorld* World = GetWorld();
		if (World == nullptr) {
			return;
		}
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AEFBomb* Bomb = World->SpawnActor<AEFBomb>(AEFBomb::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
		Bomb->StartFuse();
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

bool UEFGAPlaceBomb::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	UE_LOG(LogTemp, Log, TEXT("Can activate?"));
	return true;
}
