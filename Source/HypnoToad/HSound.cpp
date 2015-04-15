// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HSound.h"
#include "AICharacter.h"

bool HWord::Compare(const TSharedPtr<HSound> rhs)
{
	if (rhs->GetType() != HSoundType::Word)
		return false;
	const TSharedPtr<HWord> rhsWord = StaticCastSharedPtr<HWord>(rhs);
	return content == rhsWord->content;
}

bool HGunShot::Compare(const TSharedPtr<HSound> rhs)
{
	return rhs->GetType() == HSoundType::GunShot;
}

void HSound::BroadCastSound(UWorld* world, TSharedPtr<HSound> sound)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("wassup"));
	for (TActorIterator<AAICharacter> ActorItr(world); ActorItr; ++ActorItr)
	{
		if ((sound->GetOrigin() - ActorItr->GetActorLocation()).Size() < sound->m_range)
			ActorItr->HearSound(sound);
	}
}