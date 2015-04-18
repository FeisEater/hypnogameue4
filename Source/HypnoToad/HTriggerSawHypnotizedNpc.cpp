// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSawHypnotizedNpc.h"
#include "AICharacter.h"

bool HTriggerSawHypnotizedNpc::IsTriggered()
{
	for (TActorIterator<AAICharacter> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
	{
		FVector diff = ActorItr->GetActorLocation() - m_owner->GetActorLocation();
		if (diff.Size() < 5000 && ActorItr->IsHypnotized() && m_owner->CanSee(*ActorItr))
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