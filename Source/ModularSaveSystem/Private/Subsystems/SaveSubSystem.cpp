// Fill out your copyright notice in the Description page of Project Settings.


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
	return true;
}

bool USaveSubsystem::Load(const FString& SlotName, AActor* Actor)
{
	// Write the function logic This Line.
	return true;
}
