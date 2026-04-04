// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintLibrary/ModularSaveSystemBlueprintLibrary.h"

#include "Kismet/KismetStringLibrary.h"

bool UModularSaveSystemBlueprintLibrary::FStringToBoolDefault(const FString& InString, bool bDefaultValue)
{
	if (UKismetStringLibrary::EqualEqual_StriStri(InString, FString("true")))
	{
		return true;
	}
	if (UKismetStringLibrary::EqualEqual_StriStri(InString, FString("false")))
	{
		return false;
	}
	ensureMsgf(false, TEXT("Invalid FString Entered in %hs FString: %s"), __func__, *InString);
	return bDefaultValue;
}

uint8 UModularSaveSystemBlueprintLibrary::FStringToEnumByteDefault(const FString& InString, const UEnum* InEnum,
	const uint8 InDefaultValue)
{
	if (InEnum == nullptr)
	{
		ensureMsgf(false, TEXT("Invalid Enum Passed in %hs"), __func__);
		return InDefaultValue;
	}
	int64 EnumValue =  InEnum->GetValueByNameString(InString);

	if (EnumValue != INDEX_NONE)
	{
		return static_cast<uint8>(EnumValue);
	}
	
	ensureMsgf(false, TEXT("Invalid FString Entered in %hs FString: %s"), __func__, *InString);
	return InDefaultValue;
	
}
