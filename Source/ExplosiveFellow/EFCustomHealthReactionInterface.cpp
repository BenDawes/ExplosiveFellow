// Fill out your copyright notice in the Description page of Project Settings.


#include "EFCustomHealthReactionInterface.h"

// Add default functionality here for any IEFCustomHealthReactionInterface functions that are not pure virtual.
void IEFCustomHealthReactionInterface::OnHealthChange(float NewHealth)
{
	if (NewHealth < 0.01f)
	{
		UE_LOG(LogTemp, Log, TEXT("Default custom health reaction"));
		if (AActor* IsActor = Cast<AActor>(this)) {
			IsActor->Destroy();
		}
	}
}