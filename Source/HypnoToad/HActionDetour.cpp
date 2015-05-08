// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionDetour.h"
#include "AICharacter.h"

void HActionDetour::RunAction()
{
	if (m_detourPoint && FVector::Dist(*m_targetPosition, m_detourPoint->GetActorLocation()) < 200)
		return;
	if (FVector::Dist(*m_targetPosition, m_owner->GetActorLocation()) < 200)
		return;
	if (m_detourPoint)
		m_detourPoint->Destroy();
	m_detourPoint = m_owner->GetWorld()->SpawnActor<APathPoint>(*m_targetPosition, FRotator::ZeroRotator);
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
}

void HActionDetour::CollectParameters()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowActorParameterGui(plr->GetMarkedLocations());
}
