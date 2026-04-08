// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

/**
 * 
 */
USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY()// no need for SaveGame keyword since this struct will be only used as key value for a TMap UObject value which is marked with SaveGame.
	TMap<FString, FString> SaveData;

	UPROPERTY()
	int32 SaveDataVersionNumber = 0;

	FActorSaveData(){}

	FActorSaveData(const TMap<FString, FString>& InSaveData): SaveData(InSaveData){};

	FActorSaveData( const TMap<FString, FString>& InSaveData, const int32 InSaveDataVersionNumber): SaveData(InSaveData), SaveDataVersionNumber(InSaveDataVersionNumber){};
	
};


UCLASS()
class USaveGameData : public USaveGame
{
	GENERATED_BODY()

public:
 	UPROPERTY(SaveGame)
	TMap<FGuid, FActorSaveData> GameData;

};
