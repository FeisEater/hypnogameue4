// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSawHypnotizedNpc.h"
#include "AICharacter.h"

bool HTriggerSawHypnotizedNpc::IsTriggered()
{
	for (TActorIterator<AAICharacter> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsAsleep() && m_owner->CanSee(*ActorItr))
		{
			m_hypnotizedNpcLocation = ActorItr->GetActorLocation();
			return true;
		}
	}
	return false;
}

TSharedPtr<FVector> HTriggerSawHypnotizedNpc::GetHypnotizedNpcLocation()
{
	return TSharedPtr<FVector>(&m_hypnotizedNpcLocation);
}