// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * 
 */
class HYPNOTOAD_API HTriggerHeard : public HTrigger
{
public:
	HTriggerHeard(AAICharacter* owner, USound* sound) : HTrigger(owner), m_sound(sound) {}
	TSharedPtr<FVector> GetSoundSource();

	FString GetMenuName() override
	{
		return "When heard...";
	}
	HTrigger* CreateTrigger() override
	{
		return new HTriggerHeard(m_owner, m_sound);
	}
	void CollectParameters() override;
	void SetSoundParameter(USound* sound) override;

protected:
	virtual bool IsTriggered() override;

private:
	USound* m_sound;
	FVector m_soundSource;
};
