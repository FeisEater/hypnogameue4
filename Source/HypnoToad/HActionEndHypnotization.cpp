// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionEndHypnotization.h"
#include "AICharacter.h"

void HActionEndHypnotization::RunAction()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("im awake"));
	m_owner->EndHypnotization();
}