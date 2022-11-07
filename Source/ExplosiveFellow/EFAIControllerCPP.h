// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
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
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
