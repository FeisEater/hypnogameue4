// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"
#include "HSound.h"

/**
 * 
 */
class AAICharacter;

class HYPNOTOAD_API HTrigger
{
public:
	HTrigger(AAICharacter* owner) : m_owner(owner) {}

	void Trigger();

	void SetAction(HAction* action) { m_action = action; }

	virtual FString GetMenuName() = 0;
	virtual HTrigger* CreateTrigger() = 0;
	virtual void CollectParameters();
	virtual void SetSoundParameter(TSharedPtr<HSound> sound);

protected:
	virtual bool IsTriggered() { return false; }
	
	AAICharacter* m_owner;
	HAction* m_action;
};
