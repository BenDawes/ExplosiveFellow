// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "EFBomb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplodeDelegate);

UCLASS()
class EXPLOSIVEFELLOW_API AEFBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEFBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* TriggerCapsule;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool HasExploded();

	UFUNCTION(BlueprintCallable)
	void BecomeSolid();

	UFUNCTION(BlueprintCallable)
	void StartFuse();

	UFUNCTION(BlueprintCallable)
	void SetFuseTime(float NewTime);
	
	UFUNCTION(BlueprintCallable)
	void SetExplosionRadius(float NewRadius);

	UFUNCTION(BlueprintCallable)
	void SetBombDamageLevel(float NewDamageLevel);

	UFUNCTION(BlueprintCallable)
	void SetIsPenetrating(bool NewIsPenetrating);

	UFUNCTION(BlueprintNativeEvent)
	void OnExplode();
	virtual void OnExplode_Implementation();

	UPROPERTY(BlueprintAssignable)
	FOnExplodeDelegate OnExplodeDelegate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FuseTime = 2.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ExplosionRadius = 200;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BombDamageLevel = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsPenetrating = false;

	/** AI */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* BombStimulusSource;
	
	UFUNCTION()
	virtual void LocalOnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	FTimerHandle FuseTimerHandle;

	bool bIsSolid = false;

	bool bHasExploded = false;

	TSubclassOf<UGameplayEffect> DamageEffectClass;

	static bool IsGASActor(AActor* Actor);
};
