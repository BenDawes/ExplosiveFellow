// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EFAttributeSet.h"
#include "EFGameplayAbility.h"
#include "EFAbilitySystemComponent.h"
#include "EFAIControllerCPP.h"
#include "EFBomb.h"
#include <GameplayEffectTypes.h>
#include "Blueprint/UserWidget.h"
#include "ExplosiveFellow.h"

const FName AExplosiveFellowCharacter::MoveForwardBinding("MoveForward");
const FName AExplosiveFellowCharacter::MoveRightBinding("MoveRight");

AExplosiveFellowCharacter::AExplosiveFellowCharacter()
{

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create GAS component
	AbilitySystemComponent = CreateDefaultSubobject<UEFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// Create an attribute set
	AttributeSet = CreateDefaultSubobject<UEFAttributeSet>(TEXT("AttributeSet"));
}

void AExplosiveFellowCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AExplosiveFellowCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerState* PS = GetPlayerState())
	{
		PS->NetUpdateFrequency = 100;
	}
	OnActorBeginOverlap.AddDynamic(this, &AExplosiveFellowCharacter::LocalOnActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AExplosiveFellowCharacter::LocalOnActorEndOverlap);

	MoveSpeed = AttributeSet->GetMaxSpeed();
	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMaxSpeed();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxSpeedAttribute()).AddUObject(this, &AExplosiveFellowCharacter::MaxSpeedChanged);
}

void AExplosiveFellowCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AExplosiveFellowCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	AddMovementInput(MoveDirection, MoveSpeed * DeltaSeconds);
}

void AExplosiveFellowCharacter::InitializeAttributes()
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

void AExplosiveFellowCharacter::InitializeAbilities()
{
	if (HasAuthority() && AbilitySystemComponent && !AbilitySystemComponent->AbilitiesHaveBeenInitialized())
	{
		for (TSubclassOf<UEFGameplayAbility>& DefaultAbility : DefaultAbilities)
		{
			// Initialize all abilities at level 1 for now
			int32 EffectLevel = 1;
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(DefaultAbility, EffectLevel, static_cast<int32>(DefaultAbility.GetDefaultObject()->AbilityInputID), this)
			);
		}
		AbilitySystemComponent->SetAbilitiesHaveBeenInitialized(true);
	}
}

void AExplosiveFellowCharacter::SetupAbilitySystemInputBinds(UInputComponent* IC = nullptr)
{
	if (IC == nullptr) {
		IC = InputComponent;
	}
	if (AbilitySystemComponent && IC && !AbilitySystemComponent->AbilitiesHaveBeenBound())
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Confirm));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(IC, Binds);
		AbilitySystemComponent->SetAbilitiesHaveBeenBound(true);
	}
}

void AExplosiveFellowCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// (Owner, Avatar)
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	SetOwner(NewController);
	InitializeAttributes();
	InitializeAbilities();

	if (AEFAIControllerCPP* AsAI = Cast<AEFAIControllerCPP>(NewController))
	{
		SetIsAIControlled(true);
	}
	else
	{
		SetIsAIControlled(false);
	}
}

void AExplosiveFellowCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	SetupAbilitySystemInputBinds(); 
}

void AExplosiveFellowCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	SetupAbilitySystemInputBinds(PlayerInputComponent);
}

void AExplosiveFellowCharacter::OnHealthChange(float NewHealth)
{
	if (NewHealth < 0.01f)
	{
		if (bIsAIControlled)
		{
			Destroy();
		}
		else
		{
			if (OriginatingSpectatorPawn != nullptr)
			{
				GetController()->Possess(OriginatingSpectatorPawn);
				Destroy();
			}
		}
	}
}

void AExplosiveFellowCharacter::SetIsAIControlled(bool NewValue)
{
	bIsAIControlled = NewValue;
}

bool AExplosiveFellowCharacter::GetIsInsideBomb()
{
	return bIsInsideBomb;
}

void AExplosiveFellowCharacter::SetOriginatingSpectatorPawn(AEFSpectatorCPP* NewPawn)
{
	OriginatingSpectatorPawn = NewPawn;
}

void AExplosiveFellowCharacter::LocalOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AEFBomb>(OtherActor) != nullptr)
	{
		nBombsInside += 1;
		bIsInsideBomb = true;
	}
}

void AExplosiveFellowCharacter::LocalOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AEFBomb>(OtherActor) != nullptr)
	{
		nBombsInside -= 1;
	}
	if (nBombsInside <= 0)
	{
		nBombsInside = 0;
		bIsInsideBomb = false;
	}

}

void AExplosiveFellowCharacter::MaxSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	MoveSpeed = Data.NewValue * 100;
}