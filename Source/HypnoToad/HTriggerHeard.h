// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * Triggers when correct sound is heard
 */
class HYPNOTOAD_API HTriggerHeard : public HTrigger
{
public:
	HTriggerHeard(AAICharacter* owner, USound* sound) : HTrigger(owner), m_sound(sound)
	{
		//Prevent m_sound from being garbage collected
		if (m_sound)
			m_sound->AddToRoot();
	}
	~HTriggerHeard()
	{
		if (m_sound && m_sound->IsValidLowLevel())
			m_sound->ConditionalBeginDestroy();
	}

	/** Provide pointer to sound source location. */
	TSharedPtr<FVector> GetSoundSource();

	FString GetMenuName() override;
	HTrigger* CreateTrigger() override
	{
		return new HTriggerHeard(m_owner, m_sound);
	}
	void CollectParameters() override;
	void SetSoundParameter(USound* sound) override;

protected:
	virtual bool IsTriggered() override;

private:
	/** Sound to hear */
	USound* m_sound;
	/** Sound origin location */
	FVector m_soundSource;
};
