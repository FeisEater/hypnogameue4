// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionSay : public HAction
{
public:
	HActionSay(AAICharacter* owner, USound* word, bool overrideAttack = false) : HAction(owner, overrideAttack), m_sound(word)
	{
		if (m_sound)
			m_sound->AddToRoot();
	}
	FString GetMenuName() override { return "Say..."; }
	HAction* CreateAction() override { return new HActionSay(m_owner, m_sound); }
	void CollectParameters() override;
	void SetSoundParameter(USound* sound) override;

protected:
	virtual void RunAction() override;

private:
	USound* m_sound;
};
