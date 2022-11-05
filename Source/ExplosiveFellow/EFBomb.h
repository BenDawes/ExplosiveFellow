// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "EFBomb.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FuseTime = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ExplosionRadius = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float BombDamageLevel = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsPenetrating = false;

private:
	FTimerHandle FuseTimerHandle;

	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
