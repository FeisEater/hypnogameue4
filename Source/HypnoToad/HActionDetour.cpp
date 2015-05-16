// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionDetour.h"
#include "AICharacter.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"

void HActionDetour::RunAction()
{
	//If too close, don't run action.
	if (FVector::Dist(*m_targetPosition, m_owner->GetActorLocation()) < 200)
		return;
	float dist = UNavigationSystem::FindPathToLocationSynchronously(m_owner->GetWorld(), m_owner->GetActorLocation(), *m_targetPosition)->GetPathLength();
	//If detour requires too long of a path, don't detour.
	if (m_maxDistance > 0 && dist > m_maxDistance)
		return;
	//When visited temporary pathpoint, destroy it.
	if (m_detourPoint && m_detourPoint->GotVisited())
	{
		m_detourPoint->Destroy();
		m_detourPoint = NULL;
	}
	if (!m_detourPoint)
	{
		//Create temporary pathpoint.
		m_detourPoint = m_owner->GetWorld()->SpawnActor<APathPoint>(*m_targetPosition, FRotator::ZeroRotator);
		
		//Link new pathpoint to next non-temporary pathpoint in the path.
		APathPoint* NextNonOverridablePPoint = m_owner->GetNextPPoint();
		for (int i = 0; i < 1000; ++i)
		{
			if (!NextNonOverridablePPoint || !NextNonOverridablePPoint->Overridable)
				break;
			NextNonOverridablePPoint = (APathPoint*)NextNonOverridablePPoint->NextPPoint;
			if (i == 999)
				UE_LOG(LogTemp, Warning, TEXT("Seems like all ppoints are overridable, which shouldn't happen"));
		}
		m_detourPoint->NextPPoint = NextNonOverridablePPoint;

		m_detourPoint->Overridable = true;
		m_owner->SetNextPPoint(m_detourPoint);
		Consume();
	}
}

void HActionDetour::CollectParameters()
{
	//Collect marked location from gui, also other npc's
	AHypnoToadCharacter* plr = GetPlayer();
	TArray<AActor*> locations = plr->GetMarkedLocations();
	locations += plr->GetNpcAttackTargets();
	plr->ShowActorParameterGui(locations);
}
