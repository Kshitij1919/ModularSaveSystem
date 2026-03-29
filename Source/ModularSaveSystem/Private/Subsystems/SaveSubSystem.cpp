// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/SaveableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SaveGameData.h"
#include "Subsystems/SaveSubsystem.h"

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool USaveSubsystem::Save(const FString& SlotName, AActor* Actor)
{
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : SlotName is empty"));
		return false;
	}

	if (Actor == nullptr || !Actor->Implements<USaveableInterface>())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Invalid Actor passed"));
		return false;
	}

	FGuid ActorGuid = ISaveableInterface::Execute_GetSaveGuid(Actor);
	if (! ActorGuid.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Actor Guid is Invalid"));
		return false;
	}	

	TMap<FGuid, FActorSaveData> LocalGameData;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		USaveGame* OldSaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
		if (OldSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Could not Get the existing Save File"));
			return false;
		}
		USaveGameData* OldSaveGameData = Cast<USaveGameData>(OldSaveGame);
		if (OldSaveGameData == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Cast to USaveGameData failed from Old Save Game"));
			return false;
		}

		LocalGameData = OldSaveGameData->GameData;

		if (LocalGameData.Contains(ActorGuid))
		{
			LocalGameData.Remove(ActorGuid);
		}
	}

	LocalGameData.Add(ActorGuid, FActorSaveData(ISaveableInterface::Execute_GetSaveData(Actor)));

	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass());
	if (NewSaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Could not create new Save Game"));
		return false;
	}

	USaveGameData* NewSaveGameData = Cast<USaveGameData>(NewSaveGame);
	if (NewSaveGameData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Cast to USaveGameData failed from New Save Game"));
		return false;
	}

	NewSaveGameData->GameData = LocalGameData;

	if (! UGameplayStatics::SaveGameToSlot(NewSaveGameData, SlotName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Save : Could not save game to Slot: %s"), *SlotName);
		return false;
	}
	
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

	//1. 
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : SlotName is empty"));
		return false;
	}
	//2. 
	if (Actor == nullptr || !Actor->Implements<USaveableInterface>())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : Invalid Actor passed"));
		return false;
	}
	
	FGuid ActorGuid = ISaveableInterface::Execute_GetSaveGuid(Actor);
	if (! ActorGuid.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : Actor Guid is Invalid"));
		return false;
	}

	//3.
	if (! UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : Save file does not exist in Slot: %s"), *SlotName);
		return false;
	}

	//4.
	//4.1
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);

	//4.2
	if (SaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : Could not load Save "));
		return false;
	}

	USaveGameData* SaveGameData = Cast<USaveGameData>(SaveGame);
	if (SaveGameData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : cast to USaveGameData failed"));
		return false;
	}

	//4.3.2
	if (! SaveGameData->GameData.Contains(ActorGuid))
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : ACtor Guid does not exist"));
		return false;
	}

	//4.3.1
	ISaveableInterface::Execute_RestoreSaveData(Actor, SaveGameData->GameData.Find(ActorGuid)->SaveData);
	
	return true;
}
