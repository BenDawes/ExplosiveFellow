// Fill out your copyright notice in the Description page of Project Settings.


#include "EFPickUpProvider.h"
#include "EFPickUpCPP.h"

AEFPickUpProvider::AEFPickUpProvider()
{
	static ConstructorHelpers::FClassFinder<AEFPickUpCPP> HealAsset(TEXT("Class'/Script/ExplosiveFellow.EFPickUp_Heal'"));
	static ConstructorHelpers::FClassFinder<AEFPickUpCPP> SpeedBoostAsset(TEXT("Class'/Script/ExplosiveFellow.EFPickUp_TemporarySpeedBoost'"));
	static ConstructorHelpers::FClassFinder<AEFPickUpCPP> MaxBombsAsset(TEXT("Class'/Script/ExplosiveFellow.EFPickUp_IncreaseMaxBombs'"));
	static ConstructorHelpers::FClassFinder<AEFPickUpCPP> BombDamageAsset(TEXT("Class'/Script/ExplosiveFellow.EFPickUp_IncreaseBombDamage'"));
	static ConstructorHelpers::FClassFinder<AEFPickUpCPP> BombSizeAsset(TEXT("Class'/Script/ExplosiveFellow.EFPickUp_IncreaseBombRadius'"));
	static ConstructorHelpers::FClassFinder<AEFPickUpCPP> PenetratesAsset(TEXT("Class'/Script/ExplosiveFellow.EFPickUp_NextBombPenetrates'"));

	ConstructorHelpers::FClassFinder<AEFPickUpCPP> AssetsArray[] = {
		HealAsset, SpeedBoostAsset, MaxBombsAsset, BombDamageAsset, BombSizeAsset, PenetratesAsset
	};
	for (auto Finder : AssetsArray)
	{
		if (Finder.Succeeded()) {
			PickupClasses.Add(Finder.Class);
		}
	}
}

TSubclassOf<AEFPickUpCPP> AEFPickUpProvider::GetRandomPickUpClass()
{
	if (PickupClasses.Num() == 0)
	{
		return nullptr;
	}
	return PickupClasses[FMath::RandRange(0, PickupClasses.Num() - 1)];
}
