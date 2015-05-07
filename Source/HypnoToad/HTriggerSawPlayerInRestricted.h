// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * 
 */
class HYPNOTOAD_API HTriggerSawPlayerInRestricted : public HTrigger
{
public:
	HTriggerSawPlayerInRestricted(AAICharacter* owner) : HTrigger(owner) {}

	FString GetMenuName() override
	{
		return "When saw player in restricted area";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerSawPlayerInRestricted(m_owner);
	}

protected:
	virtual bool IsTriggered() override;

};
