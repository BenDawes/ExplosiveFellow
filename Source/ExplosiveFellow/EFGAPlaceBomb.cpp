// Fill out your copyright notice in the Description page of Project Settings.


#include "EFGAPlaceBomb.h"
#include "EFBomb.h"


UEFGAPlaceBomb::UEFGAPlaceBomb() {}

void UEFGAPlaceBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("Spawning bomb"));
	if ( UWorld* World = GetWorld())
	{
		if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
		{
			return;
		}
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
		FVector SpawnLocationMaxVerticalDrop = FVector(SpawnLocation);
		SpawnLocationMaxVerticalDrop.Z -= 5000;
		FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();
		FHitResult OutHit;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		bool HadHit = World->LineTraceSingleByObjectType(OutHit, SpawnLocation, SpawnLocationMaxVerticalDrop, ObjectQueryParams);
		if (HadHit) {
			SpawnLocation.Z = OutHit.ImpactPoint.Z + 50;
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
