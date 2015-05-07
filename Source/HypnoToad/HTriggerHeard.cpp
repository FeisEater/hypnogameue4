// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerHeard.h"
#include "AICharacter.h"

bool HTriggerHeard::IsTriggered()
{
	if (m_sound == NULL)
		return false;
	USound* sound = m_owner->HeardSound(m_sound);
	if (sound != NULL)
	{
		m_soundSource = sound->Origin;
		return true;
	}
	return false;
}

TSharedPtr<FVector> HTriggerHeard::GetSoundSource()
{
	return TSharedPtr<FVector>(&m_soundSource);
}

void HTriggerHeard::CollectParameters()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowSoundParameterGui();
}

void HTriggerHeard::SetSoundParameter(USound* sound)
{
	m_sound->ConditionalBeginDestroy();
	m_sound = sound;
	m_sound->AddToRoot();
	HTrigger::SetSoundParameter(sound);
}