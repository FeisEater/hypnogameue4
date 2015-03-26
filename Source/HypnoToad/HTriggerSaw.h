// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"
/**
 * 
 */
class AAICharacter;

template <class T>
class HYPNOTOAD_API HTriggerSaw : public HTrigger
{
public:
	HTriggerSaw(AAICharacter* owner) : HTrigger(owner) {}
	virtual bool IsTriggered() override
	{
		for (TActorIterator<T> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
		{
			if (m_owner->CanSee(*ActorItr))
				return true;
		}
		return false;
	}

};
