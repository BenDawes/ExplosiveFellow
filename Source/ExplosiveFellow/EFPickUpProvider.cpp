// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUpProvider.h"
#include "FWeightedAssetRow.h"
#include "EFPickUpCPP.h"

AEFPickUpProvider::AEFPickUpProvider()
{
}

TSubclassOf<AEFPickUpCPP> AEFPickUpProvider::GetRandomPickUpClass()
{
	if (PickUpClasses.Num() == 0)
	{
		return nullptr;
	}
	float UniformRand = FMath::FRand();
	float ScaledRand = UniformRand * Max;
	float Counter = 0;
	for (int i = 0; i < PickUpWeights.Num(); i++)
	{
		Counter += PickUpWeights[i];
		if (Counter > ScaledRand)
		{
			return PickUpClasses[i];
		}
	}
	return PickUpClasses[FMath::RandRange(0, PickUpClasses.Num() - 1)];
}

void AEFPickUpProvider::GenerateFromDataTable(UDataTable* Table)
{
	for (FName RowName : Table->GetRowNames())
	{
		FWeightedAssetRow* Row = Table->FindRow<FWeightedAssetRow>(RowName, TEXT("PickUp Provider Row Extraction"));
		if (Row->Actor->IsChildOf(AEFPickUpCPP::StaticClass()))
		{
			PickUpClasses.Add((TSubclassOf<AEFPickUpCPP>)Row->Actor);
			PickUpWeights.Add(Row->Weight);
		}
	}
	Max = 0;
	for (float Weight : PickUpWeights)
	{
		Max += Weight;
	}
}
