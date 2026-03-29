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

public:
	/**
	 * 
	 * @param SlotName - THe Slot name in which the Caller wishes to save the Data. 
	 * @param Actor - The Actor Calling the Save.
	 * @return - whether the Save failed or succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveSubSystem")
	bool Save(const FString& SlotName, const AActor* Actor);

	/**
	 * 
	 * @param SlotName - THe Slot name in which the Caller wishes to Load the Data from. 
	 * @param Actor - The Actor Calling the load.
	 * @return - whether the load failed or succeeded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveSubSystem")
	bool Load(const FString& SlotName, AActor* Actor);
	
};
