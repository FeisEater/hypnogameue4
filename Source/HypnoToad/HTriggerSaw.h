// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"
/**
 * 
 */

class HYPNOTOAD_API HTriggerSaw : public HTrigger
{
public:
	HTriggerSaw(AAICharacter* owner, AActor* actor) : HTrigger(owner), m_actor(actor) {}
	~HTriggerSaw()
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying trigger saw"));
	}

	FString GetMenuName() override
	{
		return "When saw...";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerSaw(m_owner, m_actor);
	}
	void CollectParameters() override;
	void SetActorParameter(AActor* actor) override;

protected:
	virtual bool IsTriggered() override;

private:
	AActor* m_actor;

};
