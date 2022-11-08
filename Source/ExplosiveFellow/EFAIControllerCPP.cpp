// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAIControllerCPP.h"
#include "EFBomb.h"
#include "EFBoxCPP.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEFAIControllerCPP::AEFAIControllerCPP()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");

	// TODO: MAKE sense own blueprint
	// Then put stimulus on BOMB!!
	// Also on the player for a different stimulus
	UAISenseConfig_Sight* SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("AISense_Sight");
	SightSenseConfig->PeripheralVisionAngleDegrees = 360.f;
	SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightSenseConfig->SightRadius = 5000.0f;
	SightSenseConfig->SightRadius = 7000.0f;

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


	if (PossessedCharacter != nullptr && PossessedCharacter->GetCharacterMovement()->GetLastUpdateVelocity().SizeSquared() < 1.f)
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

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEFAIControllerCPP::OnPerceptionUpdated);
	// PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEFAIControllerCPP::OnTargetPerceptionUpdated);
}

void AEFAIControllerCPP::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	PossessedCharacter = Cast<AExplosiveFellowCharacter>(NewPawn);
}

void AEFAIControllerCPP::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	UpdatePerception();
}

void AEFAIControllerCPP::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// UE_LOG(LogTemp, Log, TEXT("Observed at least target %s"), *Actor->GetName());
}

void AEFAIControllerCPP::UpdatePerception()
{
	if (PossessedCharacter == nullptr)
	{
		return;
	}
	TArray<AActor*> AllPerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(PerceptionComponent->GetDominantSense(), AllPerceivedActors);
	if (PossessedCharacter->GetIsInsideBomb() || AllPerceivedActors.ContainsByPredicate([](AActor* Actor) -> bool { return Cast<AEFBomb>(Actor) != nullptr; }))
	{
		AIBlackboard->SetValueAsBool(CanSeeBombKey, true);
	}
	else
	{
		AIBlackboard->ClearValue(CanSeeBombKey);
	}
	if (PossessedCharacter != nullptr)
	{
		PerceptionComponent->GetCurrentlyPerceivedActors(PerceptionComponent->GetDominantSense(), AllPerceivedActors);
		DestructibleVisibleActors = AllPerceivedActors.FilterByPredicate([](AActor* Actor) -> bool { return Cast<AEFBoxCPP>(Actor) != nullptr; });
		if (DestructibleVisibleActors.Num() > 0)
		{
			auto LocalPossessedCharacter = PossessedCharacter;
			auto NearByCandidates = DestructibleVisibleActors.FilterByPredicate([LocalPossessedCharacter](AActor* Actor) -> bool { return Actor->GetDistanceTo(LocalPossessedCharacter) < 200; });
			FVector LookVector = PossessedCharacter->GetActorRotation().RotateVector(FVector::RightVector);
			LookVector.Normalize();
			auto VaguelyFacingCandidates = NearByCandidates.FilterByPredicate([LookVector, LocalPossessedCharacter](AActor* Actor) -> bool {
				auto VectorToCandidate = (Actor->GetActorLocation() - LocalPossessedCharacter->GetActorLocation());
				VectorToCandidate.Normalize();
				return FVector::DotProduct(VectorToCandidate, LookVector) > .25;
				});
			if (VaguelyFacingCandidates.Num() > 0)
			{
				// UE_LOG(LogTemp, Log, TEXT("Facing a nearby visible destructible object"));
				AIBlackboard->SetValueAsBool(DestructibleObstacleInWayKey, true);
			}
			else
			{
				AIBlackboard->ClearValue(DestructibleObstacleInWayKey);
			}
		}
	}
}