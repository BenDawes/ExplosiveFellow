// Fill out your copyright notice in the Description page of Project Settings.


#include "EFAIControllerCPP.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

AEFAIControllerCPP::AEFAIControllerCPP()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");

	// tODO: MAKE sense own blueprint
	// Then put stimulus on BOMB!!
	// Also on the player for a different stimulus
	UAISenseConfig_Sight* SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("AISense_Sight");
	SightSenseConfig->PeripheralVisionAngleDegrees = 359.f;
	SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightSenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightSenseConfig->SightRadius = 5000.0f;


	PerceptionComponent->ConfigureSense(*SightSenseConfig);
	PerceptionComponent->SetDominantSense(*SightSenseConfig->GetSenseImplementation());

	UAIPerceptionStimuliSourceComponent* BombStimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AISense_BombStimulusSource");
	BombStimulusSource->RegisterForSense(*SightSenseConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEFAIControllerCPP::OnTargetPerceptionUpdated);
}

void AEFAIControllerCPP::BeginPlay()
{
	Super::BeginPlay();
}

void AEFAIControllerCPP::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Log, TEXT("Observed at least %s"), *Actor->GetName());
}