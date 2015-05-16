// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"
#include "Sound.h"

/**
 * Trigger in npc ai. When conditions are met, will invoke attached action, changing npc behaviour.
 */
class AAICharacter;

class HYPNOTOAD_API HTrigger
{
public:
	HTrigger(AAICharacter* owner) : m_owner(owner)
	{
		m_action = NULL;
		m_defaultAction = NULL;
	}
	virtual ~HTrigger() {}

	/** Public function to invoke triggering. */
	void Trigger();

	HAction* GetAction()
	{
		return m_action;
	}
	HAction* GetDefaultAction()
	{
		return m_defaultAction;
	}

	void SetAction(HAction* action)
	{
		m_action = action;
	}
	void SetIndefinateAction(HAction* action)
	{
		m_defaultAction = action;
		m_action = action;
	}
	/** Reset trigger */
	void DiscardTrigger();

	/** Trigger name to show in gui */
	virtual FString GetMenuName() = 0;
	/** Factory function to produce new triggers from this template. */
	virtual HTrigger* CreateTrigger() = 0;
	/** Handle parameter collecting for trigger. */
	virtual void CollectParameters();
	/** Pass sound parameter to this trigger. */
	virtual void SetSoundParameter(USound* sound);
	/** Pass actor parameter to this trigger. */
	virtual void SetActorParameter(AActor* actor);
	/** Provide parameter to be used for attaching actor. Not used. */
	virtual AActor* ProvidedParameter() { return NULL; }

protected:
	/** Overridable function that checks for correct conditions. */
	virtual bool IsTriggered() { return false; }
	
	/** Character that owns this trigger. */
	AAICharacter* m_owner;
	/** Action to be invoked if triggered. */
	HAction* m_action;
	/** If default trigger, stored default action to which revert to. */
	HAction* m_defaultAction;
	/** Shortcut function to get player */
	class AHypnoToadCharacter* GetPlayer();

private:
	/** Run default parameter collection routine if not overloaded. */
	void DefaultParameterCollection();
};
