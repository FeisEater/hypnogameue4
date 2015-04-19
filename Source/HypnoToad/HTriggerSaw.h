// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"
/**
 * 
 */

class HYPNOTOAD_API HTriggerSaw : public HTrigger
{
public:
	HTriggerSaw(AAICharacter* owner, UClass* cl) : HTrigger(owner), m_class(cl) {}

	FString GetMenuName() override
	{
		return "When saw...";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerSaw(m_owner, m_class);
	}
	void CollectParameters()
	{
		m_class = ADecalActor::StaticClass();
		HTrigger::CollectParameters();
	}

protected:
	virtual bool IsTriggered() override;

private:
	UClass* m_class;

};
