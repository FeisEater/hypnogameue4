// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTrigger.h"
#include "AICharacter.h"

void HTrigger::Trigger()
{
	if (IsTriggered() && m_action && !m_owner->IsHypnotized())
		m_action->Execute();
}

void HTrigger::CollectParameters()
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->ShowActionsGui();
}

void HTrigger::SetSoundParameter(TSharedPtr<HSound> sound)
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->ShowActionsGui();
}

void HTrigger::SetActorParameter(AActor* actor)
{
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)m_owner->GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->ShowActionsGui();
}