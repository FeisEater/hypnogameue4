// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HSound.h"
#include "AICharacter.h"

bool HWord::Compare(const HSound* rhs)
{
	if (rhs->GetType() != HSoundType::Word)
		return false;
	const HWord *rhsWord = static_cast<const HWord *>(rhs);
	return content == rhsWord->content;
}

bool HGunShot::Compare(const HSound* rhs)
{
	return rhs->GetType() == HSoundType::GunShot;
}

void HSound::BroadCastSound(UWorld* world, HSound* sound)
{
	for (TActorIterator<AAICharacter> ActorItr(world); ActorItr; ++ActorItr)
	{
		if ((sound->GetOrigin() - ActorItr->GetActorLocation()).Size() < sound->m_range)
			ActorItr->HearSound(sound);
	}
}