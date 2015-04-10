// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSaw.h"
#include "AICharacter.h"

bool HTriggerSaw::IsTriggered()
{
	for (TActorIterator<AActor> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
	{
		if ((*ActorItr)->IsA(m_class) && m_owner->CanSee(*ActorItr))
			return true;
	}
	return false;
}
