// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSlotRegistry.generated.h"

/**
 * 
 */
UCLASS()
class USaveSlotRegistry : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	TArray<FString> SlotNames;
};
