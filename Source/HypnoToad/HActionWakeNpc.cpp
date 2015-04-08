// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionWakeNpc.h"
#include "AICharacter.h"

void HActionWakeNpc::Execute()
{
	if (!m_detourPoint)
	{
		for (TActorIterator<AAICharacter> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
		{
			if (CanSee(*ActorItr))
			{
				m_detourPoint = m_owner->GetWorld()->SpawnActor<APathPoint>(ActorItr->GetActorLocation(), FRotator::ZeroRotator);
				m_detourPoint->NextPPoint = m_owner->GetNextPPoint();
				m_owner->SetNextPPoint(m_detourPoint);
				break;
			}
		}
	}
}

bool HActionWakeNpc::CanSee(AAICharacter* actor)
{
	FVector diff = actor->GetActorLocation() - m_owner->GetActorLocation();
	diff.Normalize();
	float dot = FVector::DotProduct(m_owner->GetActorRotation().Vector(), diff);
	if (dot < 0.7f)
		return false;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(m_owner);
	FHitResult Hit;

	return	m_owner->GetWorld()->LineTraceSingle(Hit, m_owner->GetActorLocation(), actor->GetActorLocation(), ECC_Visibility, Params)
		&& Hit.Actor.Get() == actor && actor->IsHypnotized();
}
