// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/SaveableInterface.h"
#include "Subsystems/SaveSubsystem.h"

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool USaveSubsystem::Save(const FString& SlotName, const AActor* Actor)
{
	// Write the function logic This Line.
	 /**
	* 	1. We need to check validity of SlotName is it empty if yes return false with log
	* 	2. we need to check if the actor is valid and implements the SaveableInerface, if not then return false with log
	* 	3. Get the Guid of the Actor using the GetSaveGuid ans store it locally in LocalGuid
	* 	4. create a local Variable of type TMap<FGuid, FActorSaveData> LocalGameData 
	* 	4. check if a Save Game exist
	* 	5. YES - then get the save game cast it to USaveGameData, cast failed return false with log
	* 		5.1 - get the GameData varaible from USaveGameData
	* 		5.2 - LocalGameData = GameData from the SaveGame
	* 		5.2 - check if the LocalGuid already exists in LocalGameData
	* 		5.3 - YES - Find and remove the Data. 
	* 	6 - Add the TMap to LocalGameData.Add(LocalGuid, Actor->GetSaveData() "we will figure out how the tmap will be converted to struct later")
    * 	8 - Create a new save game cast it to SaveGameData and set the Game Data = LocalGameData.
    * 	9 - Save game to slot 
	  */

	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : SlotName is empty"));
		return false;
	}

	if (Actor == nullptr || !Actor->Implements<ISaveableInterface>())
	
	
	return true;
}

bool USaveSubsystem::Load(const FString& SlotName, AActor* Actor)
{
	// Write the function logic This Line.

	/**
   * 	1. We need to check validity of SlotName is it empty if yes return false with log
   * 	2. we need to check if the actor is valid and implements the SaveableInerface, if not then return false with log
   * 	3. Does Save Game exist
   * 	4. YES
   * 		4.1 - load save game from slot
   * 		4.2 - check loaded save game validiy and cast it to USaveGameData, if either fails return false with log
   * 		4.3 - Get Game Data Variable from SaveGameData and check if the Actor->GetSaveGuid() exists in the TMap.
   * 			4.3.1 - Guid Found - call RestoreSaveData on Actor with passing GameData.value.SaveData retunr true wiht log.
   * 			4.3.2 - Guid Not Found - return false wiht log.
   * 	5. No - return false with log
	 */
	
	return true;
}
