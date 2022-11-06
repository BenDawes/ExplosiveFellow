// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EFAttributeSet.h"
#include "EFGameplayAbility.h"
#include "EFAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
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
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
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
		UE_LOG(LogTemp, Log, TEXT("Updating player state frequency"));
		PS->NetUpdateFrequency = 100;
	}
}

void AExplosiveFellowCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	AddMovementInput(MoveDirection, MoveSpeed * DeltaSeconds);
	/*
	if (AbilitySystemComponent->GetActivatableAbilities().Num() == 0) {
		return;
	}
	TSharedPtr<FGameplayAbilityActorInfo> ActorInfo;
	ActorInfo = AbilitySystemComponent->AbilityActorInfo;

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Can activate ability %s %s"), AbilitySystemComponent->GetActivatableAbilities()[0].Ability->CanActivateAbility(Handle, &(*ActorInfo)) ? TEXT(" yes ") : TEXT(" no "), HasAuthority() ? TEXT(" on server") : TEXT(" on client"));
		// UE_LOG(LogTemp, Log, TEXT("Ability input ID, is pressed %s %s"), *FString::FromInt(AbilitySystemComponent->GetActivatableAbilities()[0].InputID), *FString::FromInt(AbilitySystemComponent->GetActivatableAbilities()[0].InputPressed));
		AbilitySystemComponent->AbilityLocalInputPressed(AbilitySystemComponent->GetActivatableAbilities()[0].InputID);
		//AbilitySystemComponent->TryActivateAbility(AbilitySystemComponent->GetActivatableAbilities()[0].Handle);
		bool bValidInfo = (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid()) ;
		UE_LOG(LogTemp, Log, TEXT("TryActivateAbility %s %s"), bValidInfo ? TEXT ("would return false") : TEXT("would continue"), HasAuthority() ? TEXT(" on server") : TEXT(" on client"));
	}
	*/
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
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UEFGameplayAbility>& DefaultAbility : DefaultAbilities)
		{
			// Initialize all abilities at level 1 for now
			int32 EffectLevel = 1;
			UE_LOG(LogTemp, Log, TEXT("Granting Ability %s %s"), *FString::FromInt(static_cast<int32>(DefaultAbility.GetDefaultObject()->AbilityInputID)), HasAuthority() ? TEXT(" on server") : TEXT(" on client"));
			Handle = AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(DefaultAbility, EffectLevel, static_cast<int32>(DefaultAbility.GetDefaultObject()->AbilityInputID), this)
			);
		}
	}
}

void AExplosiveFellowCharacter::SetupAbilitySystemInputBinds(UInputComponent* IC = nullptr)
{
	if (IC == nullptr) {
		IC = InputComponent;
	}
	if (AbilitySystemComponent && IC)
	{
		UE_LOG(LogTemp, Log, TEXT("Character: Binding %s %s"), *GetActorLocation().ToString(), HasAuthority() ? TEXT(" on server") : TEXT(" on client"));
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Confirm));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(IC, Binds);
	}
}

void AExplosiveFellowCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Log, TEXT("Possessed %s"), *GetName());

	// (Owner, Avatar)
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	SetOwner(NewController);
	InitializeAttributes();
	InitializeAbilities();

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
		UE_LOG(LogTemp, Log, TEXT("Player is invincible"));
	}
}