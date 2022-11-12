// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "FWeightedAssetRow.generated.h"

/**
 
 */
USTRUCT(BlueprintType)
struct EXPLOSIVEFELLOW_API FWeightedAssetRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()
public:
    /** montage **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> Actor;

    /** description **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;
};
