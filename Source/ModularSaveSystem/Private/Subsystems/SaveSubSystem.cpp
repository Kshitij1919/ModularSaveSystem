// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/SaveableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SaveGameData.h"
#include "SaveGame/SaveSlotRegistry.h"
#include "Subsystems/SaveSubsystem.h"

const FString USaveSubsystem::SaveRegistryName = FString("SaveSlotRegistry");

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitializeSaveSlotNames();
}

void USaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USaveSubsystem::InitializeSaveSlotNames()
{
	if (! UGameplayStatics::DoesSaveGameExist(SaveRegistryName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Save Game with %s slot name exists - Assumed First Launch"), *SaveRegistryName);
		return; 
	}

	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveRegistryName, 0);
	if (SaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load Save Game with %s slot name "), *SaveRegistryName);
		return;
	}

	USaveSlotRegistry* SlotRegistrySaveGame = Cast<USaveSlotRegistry>(SaveGame);
	if (SlotRegistrySaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to USaveSlotRegistry Failed"));
		return;
	}

	SaveSlotNames = SlotRegistrySaveGame->SlotNames;
}

void USaveSubsystem::PersistSlotRegistry(const FString& SlotName)
{
	USaveGame* SaveGame;
	if (UGameplayStatics::DoesSaveGameExist(SaveRegistryName, 0))
	{
		SaveGame = UGameplayStatics::LoadGameFromSlot(SaveRegistryName, 0);
		if (SaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Save Game Could not be loaded at slot %s"), *SlotName);
			return;
		}
	}
	else
	{
		SaveGame = UGameplayStatics::CreateSaveGameObject(USaveSlotRegistry::StaticClass());
		if (SaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Save Game Could not be Created at slot %s"), *SlotName);
			return;
		}
	}
	
	USaveSlotRegistry* SlotRegistrySaveGame = Cast<USaveSlotRegistry>(SaveGame);
	if (SlotRegistrySaveGame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to USaveSlotRegistry Failed while trying to add new Slot Name"));
		return;
	}

	SlotRegistrySaveGame->SlotNames.Add(SlotName);
	SaveSlotNames = SlotRegistrySaveGame->SlotNames;

	if (! UGameplayStatics::SaveGameToSlot(SlotRegistrySaveGame, SaveRegistryName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not save Save Game with %s slot name "), *SlotName);
	}
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

	if (! SaveSlotNames.Contains(SlotName))
	{
		PersistSlotRegistry(SlotName);
	}
	
	return true;
}

bool USaveSubsystem::Load(const FString& SlotName, AActor* Actor)
{
	
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : SlotName is empty"));
		return false;
	}
 
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


	if (! UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : Save file does not exist in Slot: %s"), *SlotName);
		return false;
	}


	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);


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

	if (! SaveGameData->GameData.Contains(ActorGuid))
	{
		UE_LOG(LogTemp, Error, TEXT("USaveSubsystem::Load : ACtor Guid does not exist"));
		return false;
	}

	ISaveableInterface::Execute_RestoreSaveData(Actor, SaveGameData->GameData.Find(ActorGuid)->SaveData);
	
	return true;
}

TArray<FString> USaveSubsystem::GetAllSaveSlotNames() const
{
	return SaveSlotNames;
}

bool USaveSubsystem::DoesSaveSlotExistInSlotRegistry(const FString& SlotName) const
{
	return SaveSlotNames.Contains(SlotName);
}

bool USaveSubsystem::DeleteSaveSlotFromRegistry(const FString& SlotName)
{
	
	if (! DoesSaveSlotExistInSlotRegistry(SlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Removal Failed!! Save Slot name: %s Does not exist in Registry"), *SlotName);
		return false;
	}

	if (! UGameplayStatics::DoesSaveGameExist(SaveRegistryName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Removal Failed!! Could not Find SaveSlot Registry with Slot Name: %s"), *SaveRegistryName);
		return false;
	}
	
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveRegistryName, 0);
	if (SaveGame == nullptr)
	{
		UE_LOG(LogTemp,Error, TEXT("Removal Failed!! Could not load SaveSlot Registry with Slot Name: %s"), *SaveRegistryName);
		return false;
	}

	USaveSlotRegistry* SlotRegistrySaveGame = Cast<USaveSlotRegistry>(SaveGame);
	if (SlotRegistrySaveGame == nullptr)
	{
		UE_LOG(LogTemp,Error, TEXT("Removal Failed!! Cast to USaveSlotRegistry failed"));
		return false;
	}

	SlotRegistrySaveGame->SlotNames.Remove(SlotName);
	SaveSlotNames = SlotRegistrySaveGame->SlotNames;

	if (! UGameplayStatics::SaveGameToSlot(SlotRegistrySaveGame, SaveRegistryName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not Save the Updated Slot Registry"));
		return false;
	}
	
	if (! UGameplayStatics::DeleteGameInSlot(SlotName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Delete Save: %s"), *SlotName);
		return false;
	}
	
	return true;
}
