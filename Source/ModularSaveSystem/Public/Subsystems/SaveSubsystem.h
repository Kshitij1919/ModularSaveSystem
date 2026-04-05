// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSAVESYSTEM_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

#pragma region VARIABLES
private:

	/** this variable is meant to hold teh slot name for the save file which will store all the save slot names used by the Subsystem*/
	static const FString SaveRegistryName;

	/** All the Valid Save Slot Names */
	TArray<FString> SaveSlotNames;
#pragma endregion



#pragma region FUNCTIONS

#pragma region INTERNAL FUNCITONS

private:

	/** Initializes the SaveSlotNames Variable with available slot names*/
	void InitializeSaveSlotNames();

	/**Adds New Slot Names to the Registry*/
	void PersistSlotRegistry(const FString& SlotName);
	
#pragma endregion

public:
	
	/**
	 * 
	 * @param SlotName - THe Slot name in which the Caller wishes to save the Data. 
	 * @param Actor - The Actor Calling the Save.
	 * @return - whether the Save failed or succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveSubSystem")
	bool Save(const FString& SlotName, AActor* Actor);

	/**
	 * 
	 * @param SlotName - THe Slot name in which the Caller wishes to Load the Data from. 
	 * @param Actor - The Actor Calling the load.
	 * @return - whether the load failed or succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveSubSystem")
	bool Load(const FString& SlotName, AActor* Actor);


	/**
	 * Description - Returns the Array of all Available Save Slot Names
	 * @return - Save SlotNames
	 */
	UFUNCTION(BlueprintPure, Category = "SaveSubSystem")
	TArray<FString> GetAllSaveSlotNames() const;

	/**
	 * Description - Checks the Current Save slot registry for the Slot Name
	 * @param SlotName - The Slot Name to check the for
	 * @return - true or false based on the existence of the slot Name in Slot Registry. 
	 */
	UFUNCTION(BlueprintPure, Category = "SaveSubSystem")
	bool DoesSaveSlotExistInSlotRegistry(const FString& SlotName) const;

	/**
	 * Description - Delete Save Slot from Registry
	 * @param SlotName - The Slot Name to Delete 
	 * @return - true or false based on success of the removal of SlotName from Slot registry.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveSubSystem")
	bool DeleteSaveSlotFromRegistry(const FString& SlotName);
	
#pragma endregion
	
};
