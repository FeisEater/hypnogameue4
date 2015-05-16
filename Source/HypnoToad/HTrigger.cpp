// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTrigger.h"
#include "AICharacter.h"

void HTrigger::Trigger()
{
	if (IsTriggered() && m_action)// && !m_owner->IsAsleep())
		m_action->Execute();
}

void HTrigger::CollectParameters()
{
	DefaultParameterCollection();
}

void HTrigger::SetSoundParameter(USound* sound)
{
	DefaultParameterCollection();
}

void HTrigger::SetActorParameter(AActor* actor)
{
	DefaultParameterCollection();
}

void HTrigger::DefaultParameterCollection()
{
	GetPlayer()->ShowActionsGui();
}

void HTrigger::DiscardTrigger()
{
	//If has default action, revert to it
	if (m_defaultAction != NULL)
	{
		m_action = m_defaultAction;
		return;
	}
	//Otherwise mark as removed and remove later.
	m_owner->RemoveActiveTrigger(this);
}

AHypnoToadCharacter* HTrigger::GetPlayer()
{
	return (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
}