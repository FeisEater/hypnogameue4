// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"
#include "Sound.h"

/**
 * 
 */
class AAICharacter;

class HYPNOTOAD_API HTrigger
{
public:
	HTrigger(AAICharacter* owner) : m_owner(owner)
	{
		m_defaultAction = NULL;
	}

	void Trigger();

	HAction* GetAction()
	{
		return m_action;
	}
	void SetAction(HAction* action) { m_action = action; }
	void SetIndefinateAction(HAction* action)
	{
		m_defaultAction = action;
		m_action = action;
	}
	void DiscardTrigger();

	virtual FString GetMenuName() = 0;
	virtual HTrigger* CreateTrigger() = 0;
	virtual void CollectParameters();
	virtual void SetSoundParameter(USound* sound);
	virtual void SetActorParameter(AActor* actor);
	virtual AActor* ProvidedParameter() { return NULL; }

protected:
	virtual bool IsTriggered() { return false; }
	
	AAICharacter* m_owner;
	HAction* m_action;
	HAction* m_defaultAction;
};
