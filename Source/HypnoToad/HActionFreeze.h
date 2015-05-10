// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"
/**
 * 
 */
class HYPNOTOAD_API HActionFreeze : public HAction
{
public:
	HActionFreeze(AAICharacter* owner, float actionCount = 5, bool overrideAttack = false) : HAction(owner, overrideAttack)
	{
		m_actionCount = actionCount;
	}
	FString GetMenuName() override { return "Freeze in place" + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionFreeze(m_owner, m_actionCount, m_overrideAttackState); }

protected:
	virtual void RunAction() override;
};
