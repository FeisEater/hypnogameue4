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
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->StartHypnotizing();
}

void HAction::SetActorParameter(AActor* actor)
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->StartHypnotizing();
}

void HAction::SetVectorParameter(TSharedPtr<FVector> vector)
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->StartHypnotizing();
}

void HAction::SetStringParameter(FString string)
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->StartHypnotizing();
}

void HAction::SetSoundParameter(USound* sound)
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->StartHypnotizing();
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
				return;
			}
		}
	}
}

FString HAction::GetMenuName()
{
	return "(" + FString::FromInt(m_actionCount) + ")";
}