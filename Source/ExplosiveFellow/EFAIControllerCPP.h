// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ExplosiveFellowCharacter.h"
#include "EFAIControllerCPP.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API AEFAIControllerCPP : public AAIController
{
	GENERATED_BODY()

public:
	AEFAIControllerCPP();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* NewPawn) override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* AIBehavior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* AIBlackboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightSenseConfig;
private:
	FName CanSeeBombKey = FName("CanSeeBomb");
	FName DestructibleObstacleInWayKey = FName("DestructibleObstacleInWay");
	FName LastObservedBombExplodedKey = FName("HasLastObservedBombExploded");

	TWeakObjectPtr<AExplosiveFellowCharacter> PossessedCharacter;
	
	void UpdatePerception();
	UFUNCTION()
	void OnLastObservedBombExplode();
};
