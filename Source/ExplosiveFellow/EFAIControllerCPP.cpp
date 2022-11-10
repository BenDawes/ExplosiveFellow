// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAIControllerCPP.h"
#include "ExplosiveFellowCharacter.h"
#include "EFBomb.h"
#include "EFBoxCPP.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEFAIControllerCPP::AEFAIControllerCPP()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComp");

	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("AISenseConfig_SightComp");
	SightSenseConfig->PeripheralVisionAngleDegrees = 360.f;
	SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightSenseConfig->SightRadius = 5000.0f;
	SightSenseConfig->LoseSightRadius = 8000.0f;

	PerceptionComponent->ConfigureSense(*SightSenseConfig);
	PerceptionComponent->SetDominantSense(*SightSenseConfig->GetSenseImplementation());

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> AIBehaviorAsset(TEXT("BehaviorTree'/Game/TopDownCPP/Blueprints/EFAIBehaviorTree.EFAIBehaviorTree'"));
	AIBehavior = AIBehaviorAsset.Object;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void AEFAIControllerCPP::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PossessedCharacter.IsValid() && PossessedCharacter->GetCharacterMovement()->GetLastUpdateVelocity().SizeSquared() < 1.f)
	{
		UpdatePerception();
	}
}

void AEFAIControllerCPP::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(AIBehavior);
	AIBlackboard = GetBlackboardComponent();
}

void AEFAIControllerCPP::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PerceptionComponent->OnPerceptionUpdated.AddUniqueDynamic(this, &AEFAIControllerCPP::OnPerceptionUpdated);
}

void AEFAIControllerCPP::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	PossessedCharacter = MakeWeakObjectPtr(Cast<AExplosiveFellowCharacter>(NewPawn));
}

void AEFAIControllerCPP::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	UpdatePerception();
}

void AEFAIControllerCPP::OnLastObservedBombExplode()
{
	AIBlackboard->SetValueAsBool(LastObservedBombExplodedKey, true);
}

void AEFAIControllerCPP::UpdatePerception()
{
	if (!PossessedCharacter.IsValid())
	{
		return;
	}
	TArray<AActor*> AllPerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(PerceptionComponent->GetDominantSense(), AllPerceivedActors);
	auto ObservedBombs = AllPerceivedActors.FilterByPredicate([](AActor* Actor) -> bool { return Cast<AEFBomb>(Actor) != nullptr; });
	if (PossessedCharacter->GetIsInsideBomb() || ObservedBombs.Num() > 0)
	{
		AIBlackboard->SetValueAsBool(CanSeeBombKey, true);
		if (ObservedBombs.Num() > 0)
		{
			AIBlackboard->ClearValue(LastObservedBombExplodedKey);
			Cast<AEFBomb>(ObservedBombs[0])->OnExplodeDelegate.AddUniqueDynamic(this, &AEFAIControllerCPP::OnLastObservedBombExplode);
		}
	}
	else
	{
		AIBlackboard->ClearValue(CanSeeBombKey);
	}
	if (PossessedCharacter != nullptr)
	{
		PerceptionComponent->GetCurrentlyPerceivedActors(PerceptionComponent->GetDominantSense(), AllPerceivedActors);
		auto DestructibleVisibleActors = AllPerceivedActors.FilterByPredicate([](AActor* Actor) -> bool { return (Cast<AEFBoxCPP>(Actor) != nullptr) || (Cast<AExplosiveFellowCharacter>(Actor) != nullptr); });
		if (DestructibleVisibleActors.Num() > 0)
		{
			auto LocalPossessedCharacter = PossessedCharacter;
			auto NearByCandidates = DestructibleVisibleActors.FilterByPredicate([LocalPossessedCharacter](AActor* Actor) -> bool { return Actor->GetDistanceTo(LocalPossessedCharacter.Get()) < 190; });
			FVector LookVector = PossessedCharacter->GetActorRotation().RotateVector(FVector::RightVector);
			LookVector.Normalize();
			auto VaguelyFacingCandidates = NearByCandidates.FilterByPredicate([LookVector, LocalPossessedCharacter](AActor* Actor) -> bool {
				auto VectorToCandidate = (Actor->GetActorLocation() - LocalPossessedCharacter->GetActorLocation());
				VectorToCandidate.Normalize();
				return FVector::DotProduct(VectorToCandidate, LookVector) > .25;
			});
			if (VaguelyFacingCandidates.Num() > 0)
			{
				AIBlackboard->SetValueAsBool(DestructibleObstacleInWayKey, true);
			}
			else
			{
				AIBlackboard->ClearValue(DestructibleObstacleInWayKey);
			}
		}
	}
}