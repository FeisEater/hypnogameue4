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

void HAction::Consume()
{
	--m_actionCount;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::SanitizeFloat(m_actionCount));
	if (m_actionCount <= 0)
	{
		for (HTrigger* t : m_owner->GetActiveTriggers())
		{
			if (t->GetAction() == this)
			{
				t->DiscardTrigger();
			}
		}
	}
}