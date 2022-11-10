// Fill out your copyright notice in the Description page of Project Settings.


#include "EFGAPlaceBomb.h"
#include "AbilitySystemComponent.h"
#include "ExplosiveFellowCharacter.h"
#include "EFAttributeSet.h"
#include "AttributeSet.h"
#include "EFGameplayAbility.h"
#include <ExplosiveFellow/ExplosiveFellow.h>
#include "Abilities/GameplayAbilityTypes.h"
#include "EFBomb.h"


UEFGAPlaceBomb::UEFGAPlaceBomb() {
	/*static ConstructorHelpers::FClassFinder<UEFGameplayAbility> NextBombPenetratesClassFinder(TEXT("/Game/TopDownCPP/GAS/GA_NextBombPenetrates"));;
	if (NextBombPenetratesClassFinder.Succeeded())
	{
		NextBombPenetratesClass = NextBombPenetratesClassFinder.Class;
	}*/
	AbilityInputID = EGASAbilityInputID::Primary;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEFGAPlaceBomb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		return;
	}
	/*TWeakObjectPtr<AActor> SpawningActor = ActorInfo->OwnerActor;
	bool bNextBombPenetrates = false;
	if (SpawningActor.IsValid())
	{
		if (AExplosiveFellowCharacter* AsCharacter = Cast<AExplosiveFellowCharacter>(SpawningActor.Get()))
		{
			TSubclassOf<UEFGameplayAbility> LocalNextBombPenetratesClass = NextBombPenetratesClass;
			FGameplayAbilitySpec* NextBombPenetratesSpec = AsCharacter->GetAbilitySystemComponent()->GetActivatableAbilities().FindByPredicate([LocalNextBombPenetratesClass](FGameplayAbilitySpec Spec) -> bool { return Spec.Ability->GetClass() == LocalNextBombPenetratesClass->GetClass(); });
			if (NextBombPenetratesSpec != nullptr)
			{
				bNextBombPenetrates = true;
				AsCharacter->GetAbilitySystemComponent()->ClearAbility(NextBombPenetratesSpec->Handle);
			}
		}
	}*/
	// UE_LOG(LogTemp, Log, TEXT("Spawning bomb"));
	if (UWorld* World = GetWorld())
	{
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

		// Bomb->SetIsPenetrating(false);
		Bomb->StartFuse();
		nPlacedBombs += 1;
		Bomb->OnExplodeDelegate.AddDynamic(this, &UEFGAPlaceBomb::OnBombExplode);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UEFGAPlaceBomb::OnBombExplode()
{
	nPlacedBombs -= 1;
}

bool UEFGAPlaceBomb::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	/*if (!ActorInfo->AbilitySystemComponent.IsValid())
	{
		return nPlacedBombs == 0;
	}
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (const UEFAttributeSet* AsPlayerSet = ASC->GetSet<UEFAttributeSet>())
	{
		return nPlacedBombs < AsPlayerSet->GetMaxBombs();
	}*/

	return nPlacedBombs == 0;
}
