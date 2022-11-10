// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EFAttributeSet.h"
#include "EFAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EFCustomHealthReactionInterface.h"
#include "EFSpectatorCPP.h"
#include "ExplosiveFellowCharacter.generated.h"

UCLASS(Blueprintable)
class AExplosiveFellowCharacter : public ACharacter, public IAbilitySystemInterface, public IEFCustomHealthReactionInterface
{
	GENERATED_BODY()

public:
	AExplosiveFellowCharacter();
	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 700;
	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Attribute set */
	FORCEINLINE class UEFAttributeSet* GetAttributeSet() { return AttributeSet; }
	/** GAS Component */
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }



	virtual void InitializeAttributes();
	virtual void InitializeAbilities();
	virtual void SetupAbilitySystemInputBinds(UInputComponent* PlayerInputComponent);

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TArray<TSubclassOf<class UEFGameplayAbility>> DefaultAbilities;

	// Custom attribute functions
	virtual void OnHealthChange(float NewHealth) override;

	void SetIsAIControlled(bool NewValue);

	bool GetIsInsideBomb();

	UFUNCTION(BlueprintCallable)
	void SetOriginatingSpectatorPawn(AEFSpectatorCPP* NewPawn);

private:
	AEFSpectatorCPP* OriginatingSpectatorPawn;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Attribute set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UEFAttributeSet* AttributeSet;

	/** ASC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"), replicated)
	UEFAbilitySystemComponent* AbilitySystemComponent;

	bool bIsAIControlled = true;
	bool bIsInsideBomb = false;
	int nBombsInside = 0;

	UFUNCTION()
	virtual void LocalOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	virtual void LocalOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};

