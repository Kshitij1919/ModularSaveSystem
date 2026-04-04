// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModularSaveSystemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSAVESYSTEM_API UModularSaveSystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Description - Converts a string value to boolean 
	 * @param InString - string to convert into bool value
	 * @param bDefaultValue - the default boolean value to revert to if the passed in string is invalid( NOT 'true' OR 'false')
	 * @return - returns the boolean value of the passed in string if the string is valid, or returns the default boolean value.
	 */
	UFUNCTION(BlueprintPure, Category = "Modular Save System Blueprint Library")
	static bool FStringToBoolDefault(const FString& InString, bool bDefaultValue);

	/**
	 * Description - Converts a String value to Enum byte 
	 * @param InString - string to convert into enum byte
	 * @param InEnum - In Enum byte value to convert 
	 * @param InDefaultValue - the default enum byte value if the function fails to convert the string into the passed enum type byte value
	 * @return - returns the enum byte value if the InEnum and String passed in are valid. 
	 */
	UFUNCTION(BlueprintPure, Category = "Modular Save System Blueprint Library")
	static uint8 FStringToEnumByteDefault(const FString& InString, const UEnum* InEnum, const uint8 InDefaultValue);
};
