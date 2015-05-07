// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSawPlayerInRestricted.h"
#include "AICharacter.h"

bool HTriggerSawPlayerInRestricted::IsTriggered()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	return m_owner->CanSee(plr) && plr->InRestrictedArea;
}
