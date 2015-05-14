// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionSay : public HAction
{
public:
	HActionSay(AAICharacter* owner, USound* word, float actionCount = 10, bool overrideAttack = false) : HAction(owner, overrideAttack), m_sound(word)
	{
		if (m_sound)
			m_sound->AddToRoot();
		m_actionCount = actionCount;
	}
	~HActionSay()
	{
		if (m_sound && m_sound->IsValidLowLevel())
			m_sound->ConditionalBeginDestroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroying action say"));
	}

	FString GetMenuName() override;
	HAction* CreateAction() override { return new HActionSay(m_owner, m_sound, m_actionCount, m_overrideAttackState); }
	void CollectParameters() override;
	void SetSoundParameter(USound* sound) override;

protected:
	virtual void RunAction() override;

private:
	USound* m_sound;
	FTimerHandle m_timer;
};
