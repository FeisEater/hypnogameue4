// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class HYPNOTOAD_API USound : public UObject
{
	GENERATED_BODY()

public:
	USound(const FObjectInitializer& ObjectInitializer);
	virtual ~USound() {}

	UFUNCTION(BlueprintCallable, Category = "Sound")
	static void BroadCastSound(UWorld* world, USound* sound);

	bool operator==(USound* rhs)
	{
		return Compare(rhs);
	}

	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	FVector Origin;

	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	float Range;

private:
	virtual bool Compare(USound* rhs) { return false; }
};
