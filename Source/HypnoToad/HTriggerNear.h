// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * Triggers when npc reaches certain location
 */
class HYPNOTOAD_API HTriggerNear : public HTrigger
{
public:
	HTriggerNear(AAICharacter* owner, TSharedPtr<FVector> position) : HTrigger(owner), m_position(position) {} //, m_alreadyNear(false) {}
	FString GetMenuName() override
	{
		return "When near...";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerNear(m_owner, m_position);
	}
	void CollectParameters() override;
	void SetActorParameter(AActor* actor) override
	{
		//Get position from passed actor
		m_position = TSharedPtr<FVector>(new FVector(actor->GetActorLocation()));
		HTrigger::SetActorParameter(actor);
	}

protected:
	virtual bool IsTriggered() override;

private:
	//Position near which if npc is placed will trigger
	TSharedPtr<FVector> m_position;
	//bool m_alreadyNear;
};
