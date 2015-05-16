// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "HAction.h"

void HAction::Execute()
{
	if (!m_workWhileHypnotized && m_owner->IsAsleep())
		return;
	if (!m_overrideAttackState && m_owner->IsAttacking())
		return;
	RunAction();
}

void HAction::CollectParameters()
{
	DefaultParameterCollection();
}

void HAction::SetActorParameter(AActor* actor)
{
	DefaultParameterCollection();
}

void HAction::SetVectorParameter(TSharedPtr<FVector> vector)
{
	DefaultParameterCollection();
}

void HAction::SetStringParameter(FString string)
{
	DefaultParameterCollection();
}

void HAction::SetSoundParameter(USound* sound)
{
	DefaultParameterCollection();
}

void HAction::Consume()
{
	--m_actionCount;
	HTrigger* trigger = GetTrigger();
	if (trigger == NULL)
		return;
	//Show consumption event in gui
	if (trigger->GetDefaultAction() != this)
		m_owner->ShowTriggerConsumption(GetMenuName());

	if (m_actionCount <= 0)
	{
		trigger->DiscardTrigger();
	}
}

/** In superclass, define name as counter in parenthesis. */
FString HAction::GetMenuName()
{
	return "(" + FString::FromInt(m_actionCount) + ")";
}

HTrigger* HAction::GetTrigger()
{
	for (HTrigger* t : m_owner->GetActiveTriggers())
	{
		if (t->GetAction() == this)
			return t;
	}
	return NULL;
}

AHypnoToadCharacter* HAction::GetPlayer()
{
	return (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
}

void HAction::DefaultParameterCollection()
{
	GetPlayer()->StartHypnotizing();
}