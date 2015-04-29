// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound.h"
#include "Word.generated.h"

UCLASS()
class HYPNOTOAD_API UWord : public USound
{
	GENERATED_BODY()

public:
	UWord(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	FString Content;

private:
	bool Compare(USound* rhs) override;
};
