// Fill out your copyright notice in the Description page of Project Settings.

#include "Einterface.h"


// Add default functionality here for any IEinterface functions that are not pure virtual.

void IEinterface::attack()
{
	UE_LOG(LogTemp, Warning, TEXT("A Level %d enemy attacks and deals %d damage"), level, strength);
}
