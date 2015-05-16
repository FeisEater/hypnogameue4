// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * Wake up from sleep
 */
class HYPNOTOAD_API HActionEndHypnotization : public HAction
{
public:
	HActionEndHypnotization(AAICharacter* owner, float actionCount = 5, bool overrideAttack = true) : HAction(owner, overrideAttack)
	{
		m_workWhileHypnotized = true;
		m_actionCount = actionCount;
	}
	FString GetMenuName() override { return "Wake up" + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionEndHypnotization(m_owner, m_actionCount, m_overrideAttackState); }

protected:
	virtual void RunAction() override;
};
