// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODULARSAVESYSTEM_API ISaveableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @return - Should return the raw Data into the TMap<FString(Key), FString(Value)> format 
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "SaveableInterface")
	TMap<FString, FString> GetSaveData() const;

	/**
	 * @param InData - Save Data from the SaveFile. 
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "SaveableInterface")
	void RestoreSaveData(const TMap<FString, FString>& InData);

	/**
	 * This function returns a FGuid which is a unique Identifier.
	 * This Guid will be used to uniquely identify the implemented class's Save Data
	 * @note - its recommended that the function Implementor create a FGuid Variable with appropriate UPROPERTY details
	 * and set the variable in constructor(c++) or ConstructorScript(Blueprints) of the class, so that each instance has a unique ID.
	 * @return - A unique ID Associated with the Interface Implementor
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "SaveableInterface")
	FGuid GetSaveGuid() const;
	
};
