// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "GunShot.h"

UGunShot::UGunShot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Range = 5000;
}

bool UGunShot::Compare(USound* rhs)
{
	//A gunshot is a gunshot
	return rhs->IsA(UGunShot::StaticClass());
}
