// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerNear.h"
#include "AICharacter.h"

bool HTriggerNear::IsTriggered()
{
	if (m_alreadyNear && FVector::Dist(m_owner->GetActorLocation(), *m_position) > 500)
	{
		m_alreadyNear = false;
		return false;
	}
	if (!m_alreadyNear && FVector::Dist(m_owner->GetActorLocation(), *m_position) < 400)
	{
		m_alreadyNear = true;
		return true;
	}
	return false;
}

void HTriggerNear::CollectParameters()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowActorParameterGui(plr->GetMarkedLocations());
}
