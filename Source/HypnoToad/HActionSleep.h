// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * Fall asleep
 */
class HYPNOTOAD_API HActionSleep : public HAction
{
public:
	HActionSleep(AAICharacter* owner, bool overrideAttack = false) : HAction(owner, overrideAttack)
	{
		m_actionCount = 10;
	}
	FString GetMenuName() override { return "Fall asleep" + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionSleep(m_owner, m_overrideAttackState); }

protected:
	virtual void RunAction() override;
};
