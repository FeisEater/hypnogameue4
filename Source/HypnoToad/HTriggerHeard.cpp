// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerHeard.h"
#include "AICharacter.h"
#include "Word.h"
#include "GunShot.h"

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
	USound* oldSound = m_sound;
	m_sound = sound;
	m_sound->AddToRoot();
	if (oldSound->IsValidLowLevel())
		oldSound->ConditionalBeginDestroy();
	HTrigger::SetSoundParameter(sound);
}

FString HTriggerHeard::GetMenuName()
{
	FString soundName = "...";
	if (m_sound && m_sound->IsA(UWord::StaticClass()))
	{
		soundName = "word '" + ((UWord*)m_sound)->Content + "'";
	}
	if (m_sound && m_sound->IsA(UGunShot::StaticClass()))
		soundName = "gunshot";
	return "When heard " + soundName;
}
