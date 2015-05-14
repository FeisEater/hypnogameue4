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
		m_action = NULL;
		m_defaultAction = NULL;
	}
	virtual ~HTrigger()
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying trigger"));
		//if (m_action != NULL)	delete m_action;
		//if (m_defaultAction != NULL)	delete m_defaultAction;
	}

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
		//if (m_action != NULL)
		//	delete m_action;
		m_action = action;
	}
	void SetIndefinateAction(HAction* action)
	{
		//if (m_action != NULL)
		//	delete m_action;
		//if (m_defaultAction != NULL)
		//	delete m_defaultAction;
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
