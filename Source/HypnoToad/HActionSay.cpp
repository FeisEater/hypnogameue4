// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionSay.h"
#include "AICharacter.h"
#include "Word.h"

void HActionSay::RunAction()
{
	m_sound->Origin = m_owner->GetActorLocation();
	USound::BroadCastSound(m_owner->GetWorld(), m_sound);
}

void HActionSay::CollectParameters()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowTextParameterGui();
}

void HActionSay::SetSoundParameter(USound* sound)
{
	USound* oldSound = m_sound;
	m_sound = sound;
	m_sound->AddToRoot();
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
	return "Say " + soundName;
}
