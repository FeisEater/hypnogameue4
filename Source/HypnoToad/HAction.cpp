// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "HAction.h"

void HAction::Execute()
{
	if (!m_workWhileHypnotized && m_owner->IsHypnotized())
		return;
	if (!m_overrideAttackState && m_owner->IsAttacking())
		return;
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

void HAction::SetVectorParameter(TSharedPtr<FVector> vector)
{
	m_owner->AttachPendingTrigger();
}

void HAction::SetStringParameter(FString string)
{
	m_owner->AttachPendingTrigger();
}

void HAction::SetSoundParameter(USound* sound)
{
	m_owner->AttachPendingTrigger();
}