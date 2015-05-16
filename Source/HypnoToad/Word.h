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

	//Words have text content
	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	FString Content;

protected:
	bool Compare(USound* rhs) override;
};
