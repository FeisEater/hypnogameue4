// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"

/**
 * 
 */
class AAICharacter;

UCLASS(Abstract)
class HYPNOTOAD_API HTrigger
{
public:
	HTrigger(AAICharacter* owner) : m_owner(owner) {}

	void Trigger();

	void SetAction(HAction* action) { m_action = action; }

protected:
	virtual bool IsTriggered() { return false; }
	
	AAICharacter* m_owner;
	HAction* m_action;
};
