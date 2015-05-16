// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound.generated.h"

/**
 * Sound object used to manipulate ai characters.
 */

UCLASS(Blueprintable)
class HYPNOTOAD_API USound : public UObject
{
	GENERATED_BODY()

public:
	USound(const FObjectInitializer& ObjectInitializer);
	virtual ~USound() {}

	//Static function to broadcast sound to every ai character.
	UFUNCTION(BlueprintCallable, Category = "Sound")
	static void BroadCastSound(UWorld* world, USound* sound);

	//Operator used when comparing what kind of sound ai character is 'expecting'
	bool operator==(USound* rhs)
	{
		if (!IsValidLowLevel())
			return false;
		if (!rhs->IsValidLowLevel())
			return false;
		//Use virtual function to support polymorphism
		return Compare(rhs);
	}

	//Sound origin. Is not considered when comparing sound objects
	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	FVector Origin;

	//Sound range. Is not considered when comparing sound objects
	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	float Range;

protected:
	//Inherit this to define custom comparison rules.
	virtual bool Compare(USound* rhs) { return false; }
};
