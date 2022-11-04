// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EFAttributeSet.h"
#include "EFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ExplosiveFellowCharacter.generated.h"

UCLASS(Blueprintable)
class AExplosiveFellowCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AExplosiveFellowCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	/** Attribute set */
	FORCEINLINE class UEFAttributeSet* GetAttributeSet() { return AttributeSet; }
	/** GAS Component */
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual void InitializeAttributes();
	virtual void InitializeAbilities();
	virtual void SetupAbilitySystemInputBinds();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TArray<TSubclassOf<class UEFGameplayAbility>> DefaultAbilities;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	/** Attribute set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UEFAttributeSet* AttributeSet;

	/** Attribute set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UEFAbilitySystemComponent* AbilitySystemComponent;
};

