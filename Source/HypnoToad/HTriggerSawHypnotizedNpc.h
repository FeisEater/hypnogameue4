// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"

/**
 * Triggered when saw another npc sleeping
 */
class HYPNOTOAD_API HTriggerSawHypnotizedNpc : public HTrigger
{
public:
	HTriggerSawHypnotizedNpc(AAICharacter* owner) : HTrigger(owner) {}
	TSharedPtr<FVector> GetHypnotizedNpcLocation();

	FString GetMenuName() override
	{
		return "When saw a hypnotized NPC";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerSawHypnotizedNpc(m_owner);
	}

protected:
	virtual bool IsTriggered() override;

private:
	FVector m_hypnotizedNpcLocation;
};
