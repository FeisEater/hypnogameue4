// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionSleep.h"
#include "AICharacter.h"

void HActionSleep::RunAction()
{
	if (!m_owner->IsAsleep())
	{
		AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
		m_owner->Sleep(plr, false);
		Consume();
	}
}