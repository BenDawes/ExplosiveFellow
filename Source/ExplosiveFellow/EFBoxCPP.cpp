// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUpProvider.h"
#include "EFBoxCPP.h"
#include "EFAttributeSet.h"
#include "EFAbilitySystemComponent.h"
#include "ExplosiveFellowGameMode.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AEFBoxCPP::AEFBoxCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	}
	// Create GAS component
	AbilitySystemComponent = CreateDefaultSubobject<UEFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	// Create an attribute set
	AttributeSet = CreateDefaultSubobject<UEFAttributeSet>(TEXT("AttributeSet"));

	BoxStimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AISense_BoxStimulusSource");
	BoxStimulusSource->bAutoRegister = true;
}

void AEFBoxCPP::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxStimulusSource->RegisterForSense(UAISense_Sight::StaticClass());
	BoxStimulusSource->RegisterWithPerceptionSystem();
}

void AEFBoxCPP::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect) {
		// Make handle with source

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		// Spec for effect, level 1 as default
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (!SpecHandle.IsValid())
		{
			return;
		}

		FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AEFBoxCPP::OnHealthChange(float NewHealth)
{
	if (NewHealth < 0.01f)
	{
		// UE_LOG(LogTemp, Log, TEXT("Destroying a box"));
		AExplosiveFellowGameMode* GameMode = GetWorld()->GetAuthGameMode<AExplosiveFellowGameMode>();
		if (GameMode != nullptr)
		{
			if (FMath::RandRange(0, 100) < 25) {
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				auto Provider = GameMode->GetPickUpProvider();
				TSubclassOf<AEFPickUpCPP> ChosenPickUp = Provider->GetRandomPickUpClass();
				GetWorld()->SpawnActor<AEFPickUpCPP>(ChosenPickUp, GetActorLocation(), FRotator::ZeroRotator, SpawnInfo);
			}
		}
		Destroy();
	}
}

// Called when the game starts or when spawned
void AEFBoxCPP::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
}

// Called every frame
void AEFBoxCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

