// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUpCPP.h"
#include "ExplosiveFellowCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"

// Sets default values
AEFPickUpCPP::AEFPickUpCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
		StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
		StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		StaticMesh->SetCanEverAffectNavigation(false);
	}
	StaticMesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(60,0,90)));

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/TopDownCPP/Sprites/Potions/TransparentSpriteMaterial"));
	
	if (MaterialAsset.Succeeded())
	{
		Material = MaterialAsset.Object;
	}

	TextureAsset = FSoftObjectPath(TEXT("Texture2D'/Game/TopDownCPP/Sprites/Potions/Half-Black.Half-Black'"));


	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	TriggerCapsule->InitCapsuleSize(50.f, 50.f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	bReplicates = true;

}

// Called when the game starts or when spawned
void AEFPickUpCPP::BeginPlay()
{
	Super::BeginPlay();

}
void AEFPickUpCPP::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(Material, this);

	TSharedPtr<FStreamableHandle> TextureHandle = UAssetManager::Get().GetStreamableManager().RequestSyncLoad(TextureAsset);
	if (TextureHandle)
	{
		auto Asset = TextureHandle->GetLoadedAsset();
		Texture = Cast<UTexture2D>(Asset);
		if (Texture)
		{
			MaterialInstanceDynamic->SetTextureParameterValue(FName("InputTexture"), Texture);
		}
	}
	StaticMesh->SetMaterial(0, MaterialInstanceDynamic);
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
	AExplosiveFellowCharacter* AsCharacter = Cast<AExplosiveFellowCharacter>(OtherActor);
	if (AsCharacter != nullptr) 
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