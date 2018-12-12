// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Einterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEinterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGAME_API IEinterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	int32 level;
	int32 strength;

	void attack();
};
