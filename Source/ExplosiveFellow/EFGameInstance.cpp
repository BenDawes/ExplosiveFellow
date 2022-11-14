// Fill out your copyright notice in the Description page of Project Settings.


#include "EFGameInstance.h"




void UEFGameInstance::SetRoomCode(FString NewCode)
{
	RoomCode = NewCode;
}

FString UEFGameInstance::GetRoomCode() const
{
	return RoomCode;
}
