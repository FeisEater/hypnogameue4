// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"
/**
 * 
 */
class HYPNOTOAD_API HActionIgnore : public HAction
{
public:
	HActionIgnore(AAICharacter* owner, float actionCount = 30, bool overrideAttack = false) : HAction(owner, overrideAttack)
	{
		m_actionCount = actionCount;
	}
	FString GetMenuName() override { return "Ignore" + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionIgnore(m_owner, m_actionCount, m_overrideAttackState); }

protected:
	virtual void RunAction() override;

private:
	FTimerHandle m_timer;
};
