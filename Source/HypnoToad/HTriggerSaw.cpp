// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSaw.h"
#include "AICharacter.h"

bool HTriggerSaw::IsTriggered()
{
	for (TActorIterator<AActor> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
	{
		if ((*ActorItr)->IsA(m_class) && CanSee(*ActorItr))
			return true;
	}
	return false;
}

bool HTriggerSaw::CanSee(AActor* actor)
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
		&& Hit.Actor.Get() == actor;
}
