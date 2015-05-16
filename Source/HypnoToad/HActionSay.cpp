// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionSay.h"
#include "AICharacter.h"
#include "Word.h"

void HActionSay::RunAction()
{
	//Use timer for consumption.
	if (m_owner->GetWorldTimerManager().GetTimerRemaining(m_timer) <= 0)
	{
		m_sound->Origin = m_owner->GetActorLocation();
		USound::BroadCastSound(m_owner->GetWorld(), m_sound);
		m_owner->GetWorldTimerManager().SetTimer(m_timer, 1, false);
		Consume();
	}
}

void HActionSay::CollectParameters()
{
	GetPlayer()->ShowTextParameterGui();
}

void HActionSay::SetSoundParameter(USound* sound)
{
	USound* oldSound = m_sound;
	m_sound = sound;
	//Prevent new sound from being garbage collected
	m_sound->AddToRoot();
	//Force old sound deletion
	if (oldSound->IsValidLowLevel())
		oldSound->ConditionalBeginDestroy();
	HAction::SetSoundParameter(sound);
}

FString HActionSay::GetMenuName()
{
	FString soundName = "...";
	if (m_sound && m_sound->IsA(UWord::StaticClass()))
	{
		soundName = "'" + ((UWord*)m_sound)->Content + "'";
	}
	return "Say " + soundName + HAction::GetMenuName();
}
