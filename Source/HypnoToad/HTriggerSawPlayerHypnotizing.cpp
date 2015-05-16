// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSawPlayerHypnotizing.h"
#include "HypnoToadCharacter.h"
#include "AICharacter.h"

bool HTriggerSawPlayerHypnotizing::IsTriggered()
{
	AHypnoToadCharacter* plr = GetPlayer();
	if (!m_owner->CanSee(plr))
		return false;
	if (plr->IsEscorting())
		return true;
	if (plr->HasConversationWith() && plr->HasConversationWith()->IsAsleep())
		return true;
	return false;
}