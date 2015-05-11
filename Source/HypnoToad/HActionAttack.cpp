// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionAttack.h"
#include "AICharacter.h"

void HActionAttack::RunAction()
{
	if (m_target == m_owner)
	{
		for (int i = 0; i < 3; ++i)
			m_owner->Hurt(m_owner);
	}
	if (m_owner->GetEnemy() != m_target && m_owner->Armed)
	{
		m_owner->Attack(m_target);
		Consume();
	}
}

void HActionAttack::CollectParameters()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowActorParameterGui(plr->GetNpcAttackTargets());
}

void HActionAttack::SetActorParameter(AActor* actor)
{
	m_target = actor;
	HAction::SetActorParameter(actor);
}