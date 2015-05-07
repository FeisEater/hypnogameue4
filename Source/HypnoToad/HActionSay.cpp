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
	m_sound->ConditionalBeginDestroy();
	m_sound = sound;
	m_sound->AddToRoot();
	HAction::SetSoundParameter(sound);
}