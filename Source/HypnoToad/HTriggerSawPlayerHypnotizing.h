// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * 
 */
class HYPNOTOAD_API HTriggerSawPlayerHypnotizing : public HTrigger
{
public:
	HTriggerSawPlayerHypnotizing(AAICharacter* owner) : HTrigger(owner) {}

	FString GetMenuName() override
	{
		return "When saw player hypnotizing another NPC";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerSawPlayerHypnotizing(m_owner);
	}

protected:
	virtual bool IsTriggered() override;
};
