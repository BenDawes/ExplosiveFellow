// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExplosiveFellow, Log, All);

UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Primary
};