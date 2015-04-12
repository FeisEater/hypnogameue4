// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSawPlayerHypnotizing.h"
#include "HypnoToadCharacter.h"
#include "AICharacter.h"

bool HTriggerSawPlayerHypnotizing::IsTriggered()
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	return m_owner->CanSee(plr) && plr->IsHypnotizing();
}