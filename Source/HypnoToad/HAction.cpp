// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "HAction.h"

void HAction::Execute()
{
	if (m_overrideAttackState || !m_owner->IsAttacking())
		RunAction();
}

void HAction::CollectParameters()
{
	m_owner->AttachPendingTrigger();
}

void HAction::SetActorParameter(AActor* actor)
{
	m_owner->AttachPendingTrigger();
}