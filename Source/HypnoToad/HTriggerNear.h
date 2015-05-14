// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * 
 */
class HYPNOTOAD_API HTriggerNear : public HTrigger
{
public:
	HTriggerNear(AAICharacter* owner, TSharedPtr<FVector> position) : HTrigger(owner), m_position(position) {} //, m_alreadyNear(false) {}
	~HTriggerNear()
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying trigger near"));
	}
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
		m_position = TSharedPtr<FVector>(new FVector(actor->GetActorLocation()));
		HTrigger::SetActorParameter(actor);
	}

protected:
	virtual bool IsTriggered() override;

private:
	TSharedPtr<FVector> m_position;
	//bool m_alreadyNear;
};
