// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * Forget enemy
 */
class HYPNOTOAD_API HActionForgetEnemy : public HAction
{
public:
	HActionForgetEnemy(AAICharacter* owner, float actionCount = 5, bool overrideAttack = false) : HAction(owner, overrideAttack)
	{
		m_workWhileHypnotized = true;
		m_actionCount = actionCount;
	}
	FString GetMenuName() override { return "Forget enemy" + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionForgetEnemy(m_owner, m_actionCount, m_overrideAttackState); }

protected:
	virtual void RunAction() override;
};
