// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"
/**
 * Triggers when certain actor is seen
 */

class HYPNOTOAD_API HTriggerSaw : public HTrigger
{
public:
	HTriggerSaw(AAICharacter* owner, AActor* actor) : HTrigger(owner), m_actor(actor) {}

	FString GetMenuName() override;
	HTrigger* CreateTrigger() override
	{
		return new HTriggerSaw(m_owner, m_actor);
	}
	void CollectParameters() override;
	void SetActorParameter(AActor* actor) override;

protected:
	virtual bool IsTriggered() override;

private:
	/** Actor to see */
	AActor* m_actor;

};
