// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerHeard.h"
#include "AICharacter.h"

bool HTriggerHeard::IsTriggered()
{
	return m_owner->HeardSound(m_sound);
}
