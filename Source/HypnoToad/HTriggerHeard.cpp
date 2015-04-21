// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerHeard.h"
#include "AICharacter.h"

bool HTriggerHeard::IsTriggered()
{
	TSharedPtr<HSound> sound = m_owner->HeardSound(m_sound);
	if (sound.IsValid())
	{
		m_soundSource = sound->GetOrigin();
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

void HTriggerHeard::SetSoundParameter(TSharedPtr<HSound> sound)
{
	m_sound = sound;
	HTrigger::SetSoundParameter(sound);
}