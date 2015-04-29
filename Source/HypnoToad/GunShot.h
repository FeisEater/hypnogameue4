// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound.h"
#include "GunShot.generated.h"

UCLASS()
class HYPNOTOAD_API UGunShot : public USound
{
	GENERATED_BODY()

public:
	UGunShot(const FObjectInitializer& ObjectInitializer);
	//virtual ~HGunShot() { UE_LOG(LogTemp, Warning, TEXT("gunshot destructed")); }

private:
	bool Compare(USound* rhs) override;
};