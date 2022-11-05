// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplosiveFellowPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ExplosiveFellowCharacter.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

const FName AExplosiveFellowPlayerController::MoveForwardBinding("MoveForward");
const FName AExplosiveFellowPlayerController::MoveRightBinding("MoveRight");

AExplosiveFellowPlayerController::AExplosiveFellowPlayerController()
{
}

void AExplosiveFellowPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;
	USceneComponent* PlayerRootComponent = GetPawn()->GetRootComponent();


	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		PlayerRootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			PlayerRootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
}

void AExplosiveFellowPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	check(InputComponent);

	// set up gameplay key bindings
	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);
}

