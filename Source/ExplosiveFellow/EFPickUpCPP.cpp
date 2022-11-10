// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUpCPP.h"
#include "ExplosiveFellowCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEFPickUpCPP::AEFPickUpCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
		StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		StaticMesh->SetCanEverAffectNavigation(false);
	}

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	TriggerCapsule->InitCapsuleSize(50.f, 50.f);
	TriggerCapsule->SetRelativeLocation(FVector(0, 0, 50.f));
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEFPickUpCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEFPickUpCPP::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	OnActorBeginOverlap.AddDynamic(this, &AEFPickUpCPP::LocalOnActorBeginOverlap);
}

// Called every frame
void AEFPickUpCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEFPickUpCPP::LocalOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (AExplosiveFellowCharacter* AsCharacter = Cast<AExplosiveFellowCharacter>(OtherActor))
	{
		OnPickUp(AsCharacter);
	}
}

void AEFPickUpCPP::SetAmount(float NewAmount)
{
	Amount = NewAmount;
}

void AEFPickUpCPP::ApplyStandardEffect(AExplosiveFellowCharacter* ReceivingCharacter, bool bWithAmount)
{
	UAbilitySystemComponent* AbilitySystemComponent = ReceivingCharacter->GetAbilitySystemComponent();
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, EffectContext);
	if (SpecHandle.IsValid())
	{
		if (bWithAmount)
		{
			SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(EffectAmountTagName), Amount);
		}
		FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AEFPickUpCPP::OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter)
{
	ApplyStandardEffect(ReceivingCharacter);
	Destroy();
}