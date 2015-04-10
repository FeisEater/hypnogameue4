// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSawHypnotizedNpc.h"
#include "AICharacter.h"

bool HTriggerSawHypnotizedNpc::IsTriggered()
{
	for (TActorIterator<AAICharacter> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsHypnotized() && m_owner->CanSee(*ActorItr))
			return true;
	}
	return false;
}
