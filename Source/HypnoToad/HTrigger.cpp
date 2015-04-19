// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTrigger.h"
#include "AICharacter.h"

void HTrigger::Trigger()
{
	if (IsTriggered() && m_action && !m_owner->IsHypnotized())
		m_action->Execute();
}

void HTrigger::CollectParameters()
{
	m_owner->AttachPendingTrigger();
}