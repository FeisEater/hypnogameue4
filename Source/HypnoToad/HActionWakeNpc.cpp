// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionWakeNpc.h"
#include "AICharacter.h"

void HActionWakeNpc::RunAction()
{
	if (m_detourPoint)
	{
		if (m_detourPoint->GotVisited())
		{
			m_owner->GetWorld()->DestroyActor(m_detourPoint);
			m_detourPoint = NULL;
		}
	}
	else
	{
		for (TActorIterator<AAICharacter> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
		{
			FVector diff = ActorItr->GetActorLocation() - m_owner->GetActorLocation();
			if (diff.Size() < 5000 && ActorItr->IsHypnotized() && m_owner->CanSee(*ActorItr))
			{
				m_detourPoint = m_owner->GetWorld()->SpawnActor<APathPoint>(ActorItr->GetActorLocation(), FRotator::ZeroRotator);
				m_detourPoint->NextPPoint = m_owner->GetNextPPoint();
				m_owner->SetNextPPoint(m_detourPoint);
				break;
			}
		}
	}
}
