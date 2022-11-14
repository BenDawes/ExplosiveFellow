// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSIVEFELLOW_API UEFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetRoomCode(FString NewCode);
	UFUNCTION(BlueprintCallable)
	FString GetRoomCode() const;
private:
	FString RoomCode;
};
