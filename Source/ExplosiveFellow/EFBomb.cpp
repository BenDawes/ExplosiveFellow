// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "EFBomb.h"
#include "UObject/WeakObjectPtrTemplates.h"

// Sets default values
AEFBomb::AEFBomb()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
	}
	static ConstructorHelpers::FClassFinder<UGameplayEffect> DamageEffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_BombDamage"));;
	if (DamageEffectClassFinder.Succeeded())
	{
		DamageEffectClass = DamageEffectClassFinder.Class;
	}
}

// Called when the game starts or when spawned
void AEFBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEFBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEFBomb::StartFuse()
{
	if (UWorld* World = GetWorld()) {
		World->GetTimerManager().SetTimer(FuseTimerHandle, this, &AEFBomb::OnExplode, FuseTime, false);
	}
}

void AEFBomb::SetFuseTime(float NewTime)
{
	FuseTime = NewTime;
}

void AEFBomb::SetExplosionRadius(float NewRadius)
{
	ExplosionRadius = NewRadius;
}

void AEFBomb::SetBombDamageLevel(float NewDamageLevel)
{
	BombDamageLevel = NewDamageLevel;
}

void AEFBomb::SetIsPenetrating(bool NewIsPenetrating)
{
	bIsPenetrating = NewIsPenetrating;
}

void AEFBomb::OnExplode_Implementation()
{
	if (UWorld* World = GetWorld()) {
		TArray<struct FOverlapResult> OutOverlaps;
		FCollisionObjectQueryParams AffectedObjectQueryParams;
		AffectedObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		AffectedObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		AffectedObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
		
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(ExplosionRadius);
		FVector BombSource = GetActorLocation();
		World->OverlapMultiByObjectType(OutOverlaps, BombSource, FQuat::Identity, AffectedObjectQueryParams, CollisionShape);

		TArray<AActor*> AffectedActorsWithGASApplied;
		AffectedActorsWithGASApplied.Reserve(OutOverlaps.Num());

		for (FOverlapResult Overlap : OutOverlaps)
		{
			if (Overlap.GetActor()->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass())) {
				AffectedActorsWithGASApplied.AddUnique(Overlap.GetActor());
			}
		}

		TArray<AActor*> DefinitelyAffectedActors;;
		DefinitelyAffectedActors.Reserve(AffectedActorsWithGASApplied.Num());
		for (AActor* PotentiallyAffectedActor : AffectedActorsWithGASApplied)
		{
			// Raycast to it; if object is inbetween, don't apply
			TArray<FHitResult> OutHits;

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
			bool HadHit = World->LineTraceMultiByObjectType(OutHits, BombSource, PotentiallyAffectedActor->GetActorLocation(), ObjectQueryParams);
			bool PassedCollisionChecks = true;
			if (bIsPenetrating)
			{
				bool AllHitsWereOtherAffectedObjects = !OutHits.ContainsByPredicate([AffectedActorsWithGASApplied](FHitResult Result) -> bool { return !AffectedActorsWithGASApplied.Contains(Result.Actor); });
				PassedCollisionChecks &= !AllHitsWereOtherAffectedObjects;
			}
			else
			{
				OutHits.Sort([](FHitResult R1, FHitResult R2) -> bool {
					//UE_LOG(LogTemp, Log, TEXT("R1 %s"), *R1.Actor->GetName());
					//UE_LOG(LogTemp, Log, TEXT("R2 %s"), *R2.Actor->GetName());
					//UE_LOG(LogTemp, Log, TEXT("R1 smaller? %s"), R1.Distance < R2.Distance ? *FString("Yes") : *FString("No"));
					return R1.Distance < R2.Distance; });
				for (FHitResult Result : OutHits)
				{
					//UE_LOG(LogTemp, Log, TEXT("Actor %s"), *Result.Actor->GetName());
				}

				auto ResultWasNotPawn = [](FHitResult Result) -> bool { return !Cast<APawn>(Result.Actor); };
				FHitResult* FirstNonPlayerHit = OutHits.FindByPredicate(ResultWasNotPawn);
				bool FirstNonPlayerHitWasThis = FirstNonPlayerHit != nullptr && FirstNonPlayerHit->Actor == PotentiallyAffectedActor;
				PassedCollisionChecks &= FirstNonPlayerHitWasThis;
			}
			if (!HadHit || PassedCollisionChecks)
			{
				DefinitelyAffectedActors.AddUnique(PotentiallyAffectedActor);
			}
		}

		for (AActor* DefinitelyAffectedActor : DefinitelyAffectedActors)
		{
			IAbilitySystemInterface* AsAbilitySystemInterface = Cast<IAbilitySystemInterface>(DefinitelyAffectedActor);
			UAbilitySystemComponent* AbilitySystemComponent = AsAbilitySystemInterface->GetAbilitySystemComponent();
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, BombDamageLevel, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	Destroy();
}

