// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "ExplosiveFellowCharacter.h"
#include "EFPickUpCPP.generated.h"

UCLASS()
class EXPLOSIVEFELLOW_API AEFPickUpCPP : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* TriggerCapsule;
public:	
	// Sets default values for this actor's properties
	AEFPickUpCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void LocalOnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual void OnPickUp(AExplosiveFellowCharacter* ReceivingCharacter);

	TSubclassOf<UGameplayEffect> EffectClass;

	void SetAmount(float NewAmount);
	float Amount = 0.f;

	FName EffectAmountTagName;
	void ApplyStandardEffect(AExplosiveFellowCharacter* ReceivingCharacter, bool bWithAmount = true);
};
