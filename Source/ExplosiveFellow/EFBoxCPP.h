// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "EFAttributeSet.h"
#include "EFAbilitySystemComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "EFCustomHealthReactionInterface.h"
#include "EFBoxCPP.generated.h"

UCLASS()
class EXPLOSIVEFELLOW_API AEFBoxCPP : public AActor, public IAbilitySystemInterface, public IEFCustomHealthReactionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEFBoxCPP();
	/** GAS Component */
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual void InitializeAttributes();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// Custom attribute functions
	virtual void OnHealthChange(float NewHealth) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
private:

	/** Attribute set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UEFAttributeSet* AttributeSet;

	/** ASC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UEFAbilitySystemComponent* AbilitySystemComponent;

	/** AI */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* BoxStimulusSource;

};
