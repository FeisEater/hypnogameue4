// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerNear.h"
#include "AICharacter.h"

bool HTriggerNear::IsTriggered()
{
	/*if (m_alreadyNear && FVector::Dist(m_owner->GetActorLocation(), *m_position) > 500)
	{
		m_alreadyNear = false;
		return false;
	}
	if (!m_alreadyNear && FVector::Dist(m_owner->GetActorLocation(), *m_position) < 400)
	{
		m_alreadyNear = true;
		return true;
	}*/
	return FVector::Dist(m_owner->GetActorLocation(), *m_position) < 200;
}

void HTriggerNear::CollectParameters()
{
	//Collect marked location actor as parameter
	GetPlayer()->ShowActorParameterGui(GetPlayer()->GetMarkedLocations());
}
