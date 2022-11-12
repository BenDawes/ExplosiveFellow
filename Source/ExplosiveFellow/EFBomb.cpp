// Fill out your copyright notice in the Description page of Project Settings.

#include "EFBomb.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEFBomb::AEFBomb()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	static ConstructorHelpers::FClassFinder<AActor> ExplosionIndicatorFinder(TEXT("/Game/TopDownCPP/Blueprints/ExplosionIndicator"));
	if (ExplosionIndicatorFinder.Succeeded())
	{
		AExplosionIndicator = ExplosionIndicatorFinder.Class;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
		StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		StaticMesh->SetCanEverAffectNavigation(false);
	}
	static ConstructorHelpers::FClassFinder<UGameplayEffect> DamageEffectClassFinder(TEXT("/Game/TopDownCPP/GAS/GE_BombDamage"));;
	if (DamageEffectClassFinder.Succeeded())
	{
		DamageEffectClass = DamageEffectClassFinder.Class;
	}
	BombStimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AISense_BombStimulusSource");
	BombStimulusSource->bAutoRegister = true;
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	TriggerCapsule->InitCapsuleSize(50.f, 50.f);
	TriggerCapsule->SetRelativeLocation(FVector(0, 0, 50.f));
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
}

void AEFBomb::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BombStimulusSource->RegisterForSense(UAISense_Sight::StaticClass());
	BombStimulusSource->RegisterWithPerceptionSystem();
	OnActorEndOverlap.AddDynamic(this, &AEFBomb::LocalOnActorEndOverlap);
}

void AEFBomb::LocalOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bIsSolid)
	{
		return;
	}
	TSet<AActor*> OutActors;
	GetOverlappingActors(OutActors);
	if (!OutActors.Array().ContainsByPredicate([](AActor* Actor) -> bool { return Cast<APawn>(Actor) != nullptr; }))
	{
		BecomeSolid();
	}
}

// Called when the game starts or when spawned
void AEFBomb::BeginPlay()
{
	Super::BeginPlay();
}

void AEFBomb::BecomeSolid()
{
	StaticMesh->SetCollisionProfileName(FName("BlockAll"));
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	StaticMesh->SetCanEverAffectNavigation(true);
	bIsSolid = true;
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
		World->GetTimerManager().SetTimer(ShowIndicatorTimerHandle, this, &AEFBomb::OnShowIndicator, FuseTime-0.2f, false);
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

void AEFBomb::OnShowIndicator_Implementation()
{
	if (UWorld* World = GetWorld()) {
		auto Indicator = World->SpawnActor<AActor>(AExplosionIndicator, GetActorLocation(), FRotator::ZeroRotator);
		Indicator->SetActorScale3D(FVector(ExplosionRadius/50.f, ExplosionRadius/50.f, 1.f));
	}
}

void AEFBomb::OnExplode_Implementation()
{
	if (bHasExploded) {
		return;
	}
	bHasExploded = true;
	if (UWorld* World = GetWorld()) {
		World->GetTimerManager().ClearTimer(FuseTimerHandle);
		World->GetTimerManager().ClearTimer(ShowIndicatorTimerHandle);
		StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		StaticMesh->SetCanEverAffectNavigation(false);
		TArray<struct FOverlapResult> OutOverlaps;
		FCollisionObjectQueryParams AffectedObjectQueryParams;
		AffectedObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		AffectedObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		AffectedObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
		
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(ExplosionRadius);
		FVector BombSource = GetActorLocation();
		FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
		CollisionQueryParams.AddIgnoredActor(this);
		World->OverlapMultiByObjectType(OutOverlaps, BombSource, FQuat::Identity, AffectedObjectQueryParams, CollisionShape, CollisionQueryParams);


		TArray<AActor*> DefinitelyAffectedActors;;
		DefinitelyAffectedActors.Reserve(OutOverlaps.Num());
		for (FOverlapResult Overlap : OutOverlaps)
		{
			AActor* PotentiallyAffectedActor = Overlap.GetActor();
			// Raycast to it; if object is inbetween, don't apply
			TArray<FHitResult> OutHits;

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
			
			bool HadHit = World->LineTraceMultiByObjectType(OutHits, BombSource, PotentiallyAffectedActor->GetActorLocation(), ObjectQueryParams, CollisionQueryParams);
			OutHits.Sort([](FHitResult R1, FHitResult R2) -> bool { return R1.Distance < R2.Distance; });
			bool PassedCollisionChecks = true;

			TArray<FHitResult> FilteredHits;
			FilteredHits = OutHits.FilterByPredicate([](FHitResult Result) -> bool {
				if (AEFBomb* AsBomb = Cast<AEFBomb>(Result.Actor)) {
					return !AsBomb->HasExploded();
				}
				return true;
			});
			if (bIsPenetrating)
			{
				bool AllHitsWereGASActors = !FilteredHits.ContainsByPredicate([](FHitResult Result) -> bool { return !AEFBomb::IsGASActor(&(*Result.Actor)); });
				PassedCollisionChecks &= AllHitsWereGASActors;
			}
			else
			{
				auto ResultWasNotPawn = [](FHitResult Result) -> bool { return !Cast<APawn>(Result.Actor); };
				FHitResult* FirstNonPlayerHit = FilteredHits.FindByPredicate(ResultWasNotPawn);
				bool FirstNonPlayerHitWasThis = FirstNonPlayerHit != nullptr && FirstNonPlayerHit->Actor == PotentiallyAffectedActor;
				PassedCollisionChecks &= FirstNonPlayerHitWasThis;
			}
			if (!HadHit || PassedCollisionChecks)
			{
				DefinitelyAffectedActors.AddUnique(PotentiallyAffectedActor);
			}
		}

		TArray<AActor*> AffectedActorsWithGASApplied;
		AffectedActorsWithGASApplied.Reserve(DefinitelyAffectedActors.Num());

		TArray<AEFBomb*> AffectedBombs;
		AffectedBombs.Reserve(DefinitelyAffectedActors.Num());

		for (AActor* DefinitelyAffectedActor : DefinitelyAffectedActors)
		{
			if (IsGASActor(DefinitelyAffectedActor)) {
				AffectedActorsWithGASApplied.AddUnique(DefinitelyAffectedActor);
			}
			if (AEFBomb* AsBomb = Cast<AEFBomb>(DefinitelyAffectedActor)) {
				AffectedBombs.AddUnique(AsBomb);
			}
		}
		for (AActor* GASActor : AffectedActorsWithGASApplied)
		{
			IAbilitySystemInterface* AsAbilitySystemInterface = Cast<IAbilitySystemInterface>(GASActor);
			UAbilitySystemComponent* AbilitySystemComponent = AsAbilitySystemInterface->GetAbilitySystemComponent();
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, BombDamageLevel, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}

		}
		for (AEFBomb* OtherBomb: AffectedBombs)
		{
			OtherBomb->OnExplode();
		}
	}
	OnExplodeDelegate.Broadcast();
	Destroy();
}

bool AEFBomb::IsGASActor(AActor* Actor)
{
	return Actor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass());
}

bool AEFBomb::HasExploded()
{
	return bHasExploded;
}

