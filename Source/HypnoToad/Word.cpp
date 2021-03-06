// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "Word.h"

UWord::UWord(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Range = 1500;
}

bool UWord::Compare(USound* rhs)
{
	//Two words are same if they have same text content
	if (!rhs->IsA(UWord::StaticClass()))
		return false;
	UWord* rhsWord = (UWord*)rhs;
	return Content == rhsWord->Content;
}
