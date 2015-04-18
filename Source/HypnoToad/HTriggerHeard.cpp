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